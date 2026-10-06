#include "FController.h"
#include <iomanip>
#include <iostream>
#include <chrono>
#include <cmath>
#include <map>
#include <thread>
#include <vector>

#include <mavsdk/plugins/mission/mission.hpp>

using namespace mavsdk;
using namespace std::chrono_literals;


FController::FController(bool isSimulation) :  QObject(),
    m_isSim(isSimulation)
{
    Mavsdk::Configuration configuration(ComponentType::CompanionComputer);

    // Essential settings for ArduPilot companion computers:
    configuration.set_system_id(1);                      // Typically 1 for companion computer
    configuration.set_component_id(MAV_COMP_ID_ONBOARD_COMPUTER);  // Standard component ID
    configuration.set_always_send_heartbeats(true);     // Maintain connection

    m_mavsdk = std::make_unique<mavsdk::Mavsdk>(configuration);

}

FController::~FController()
{
    if (m_offboard) {
        m_offboard->stop();
    }
}







//INITILIZE ETC.-----------------------------------------------------------------------------

bool FController::connect() {

    std::string connection_url = "";
    if(m_isSim){
        connection_url = "udp://:14550";
    }else{
        connection_url = "serial:///dev/ttyUSB0:57600";
    }

    // 1. Bağlantıyı kur
    auto connection_result = m_mavsdk->add_any_connection("serial:///dev/cu.usbserial-D30K1O7S:57600");
    if (connection_result != ConnectionResult::Success) {
        std::cerr << "Bağlantı hatası: " << connection_result << std::endl;
        return false;
    }

    // 2. Sistemi bekle (5 saniye timeout)
    std::cout << "Sistem bekleniyor..." << std::endl;
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < std::chrono::seconds(5)) {
        auto systems = m_mavsdk->systems();
        if (!systems.empty() && systems[0]->has_autopilot()) {
            m_system = systems[0];
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!m_system) {
        std::cerr << "Sistem bulunamadı (timeout)" << std::endl;
        return false;
    }

    if (m_system) {
        std::cout << "System ID: " << m_system->get_system_id() << std::endl;
        std::cout << "Is connected: " << (m_system->is_connected() ? "yes" : "no") << std::endl;
    }


    // 3. Plugin'leri başlat
    m_action = std::make_shared<Action>(*m_system);
    m_telemetry = std::make_shared<Telemetry>(*m_system);
    m_param = std::make_shared<Param>(*m_system);
    m_offboard = std::make_shared<Offboard>(*m_system);
    m_mavlink = std::make_shared<MavlinkPassthrough>(*m_system);

    if (m_telemetry) {
        std::cout << "Telemetry Plugin OK" << std::endl;
    } else {
        std::cout << "Telemetry Plugin FAILED" << std::endl;
    }

    subscribeTelemetry();


    return true;
}





bool FController::setInitialParameters() {
    // Parameters and their int values
    const std::vector<std::pair<std::string, int>> safety_params_int = {
        {"FRAME_CLASS", 1},
        {"FRAME_TYPE", 1},

        {"ARMING_CHECK", 0},
        {"EK3_GPS_CHECK", 0},



        {"FS_CRASH_CHECK", 0},
        {"FS_THR_ENABLE", 0},
        {"FS_GCS_ENABLE", 0},
        {"FLOW_TYPE", 5},
    };

    const std::vector<std::pair<std::string, float>> safety_params_float = {

        {"GPS_TYPE", 1},
        {"AHRS_GPS_USE", 1},
        {"AHRS_EKF_TYPE", 3},

        {"EK3_FLOW_USE", 1},
        {"EK3_ENABLE", 1},
    };

    for (const auto& param : safety_params_int) {
        auto result = m_param->set_param_int(param.first, param.second);
        if (result != mavsdk::Param::Result::Success) {
            std::cerr << "Warning: Failed to set parameter " << param.first
                      << ": " << (result) << "\n";
        }
    }

    for (const auto& param : safety_params_float) {
        auto result = m_param->set_param_float(param.first, param.second);
        if (result != mavsdk::Param::Result::Success) {
            std::cerr << "Warning: Failed to set parameter " << param.first
                      << ": " << (result) << "\n";
        }
    }

    std::this_thread::sleep_for(2s); // Wait for reboot if needed
    return true;
}













//ACTIONS-------------------------------------------------------------------------


bool FController::armIfNeeded() {
    if (!m_telemetry->armed()) {
        std::cout << "Arming...\n";
        if (m_action->arm() != Action::Result::Success) {
            std::cerr << "Failed to arm.\n";
            return false;
        }
        std::cout << "Armed.\n";
        return true;
    }else{
        std::cout << "Already armed.\n";
    }
    return true;
}

bool FController::isArmed()
{
    return m_telemetry->armed();
}

float FController::getTakeoffAltitude()
{
    return m_action->get_takeoff_altitude().second;
}

float FController::setTakeoffAltitude(float alt)
{
    m_action->set_takeoff_altitude(alt);
    return m_action->get_takeoff_altitude().second;
}















//GENERIC--------------------------------------------------------

bool FController::setFlightMode(Telemetry::FlightMode target_mode)
{
    uint32_t custom_mode = 0;

    switch (target_mode) {
    case Telemetry::FlightMode::Stabilized: custom_mode = 0; break;  // STABILIZE
    case Telemetry::FlightMode::Acro: custom_mode = 1; break;
    case Telemetry::FlightMode::Altctl: custom_mode = 2; break;      // ALT_HOLD
    case Telemetry::FlightMode::Posctl: custom_mode = 3; break;      // GUIDED ya da POSCTL
    case Telemetry::FlightMode::Hold: custom_mode = 4; break;
    case Telemetry::FlightMode::Mission: custom_mode = 5; break;
    case Telemetry::FlightMode::ReturnToLaunch: custom_mode = 6; break;
    case Telemetry::FlightMode::Land: custom_mode = 9; break;
    case Telemetry::FlightMode::Takeoff: custom_mode = 17; break;
    default:
        std::cerr << "[Mode] Desteklenmeyen uçuş modu: " << static_cast<int>(target_mode) << "\n";
        return false;
    }

    mavlink_message_t msg{};
    mavlink_msg_command_long_pack(
        m_mavlink->get_our_sysid(),
        m_mavlink->get_our_compid(),
        &msg,
        m_mavlink->get_target_sysid(),
        MAV_COMP_ID_AUTOPILOT1,
        MAV_CMD_DO_SET_MODE,
        0,
        MAV_MODE_FLAG_CUSTOM_MODE_ENABLED,
        static_cast<float>(custom_mode),
        0, 0, 0, 0, 0
        );

    m_mavlink->send_message(msg);
    std::cout << "[Mode] Mod komutu gönderildi: custom_mode = " << custom_mode << "\n";

    constexpr int max_wait_ms = 2000;
    constexpr int step_ms = 200;
    int waited = 0;

    while (waited < max_wait_ms) {
        auto current_mode = m_telemetry->flight_mode();
        std::cerr << "[Mode] current_mode: " << static_cast<int>(current_mode) << "\n";
        if (current_mode == target_mode) {
            std::cout << "[Mode] Başarıyla moda geçildi: " << static_cast<int>(target_mode) << "\n";
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(step_ms));
        waited += step_ms;
    }

    std::cerr << "[Mode] Hedef moda geçilemedi: " << static_cast<int>(target_mode) << "\n";
    return false;
}


void FController::subscribeTelemetry()
{
    std::cout << "subscribeGPSStatus" << std::endl;

    m_telemetry->subscribe_gps_info([this](Telemetry::GpsInfo gps_info) { pushGPSStat(gps_info);});
    m_telemetry->subscribe_position([this](Telemetry::Position position) {pushPosition( position);});
    m_telemetry->subscribe_attitude_euler([this](Telemetry::EulerAngle eulerAngle) {pushYPR(eulerAngle);
    });

    std::cout << "subscribeGPSStatus---" << std::endl;

}

void FController::pushGPSStat(Telemetry::GpsInfo gps_info)
{
    m_telemData.satCount = gps_info.num_satellites;
    emit sgn_pushGPSInfo(gps_info.num_satellites);
}

void FController::pushPosition(mavsdk::Telemetry::Position pos)
{
    std::cout << "lat: " << pos.latitude_deg;

    m_currentLat = pos.latitude_deg;
    m_currentLong = pos.longitude_deg;
    m_currentRelAlt = pos.relative_altitude_m;
    m_telemData.latitude = pos.latitude_deg;
    m_telemData.longitute = pos.longitude_deg;
    m_telemData.relAltitude = pos.relative_altitude_m;
    m_telemData.absAltitude = pos.absolute_altitude_m;

    emit sgn_pushGPSData(pos.latitude_deg, pos.longitude_deg, pos.absolute_altitude_m, pos.relative_altitude_m);
}

void FController::pushYPR(mavsdk::Telemetry::EulerAngle eulerAngle)
{
    m_telemData.yaw = eulerAngle.yaw_deg;
    m_telemData.pitch = eulerAngle.pitch_deg;
    m_telemData.roll = eulerAngle.roll_deg;
    emit sgn_pushYPR(eulerAngle.yaw_deg, eulerAngle.pitch_deg, eulerAngle.roll_deg);
}

void FController::test0_async()
{

    std::cout << "Disarming....\n";

    forceDisarm();
    std::cout << "waiting....\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));  // 10Hz

    m_action->set_takeoff_altitude(4);
    float takeoffAlt = m_action->get_takeoff_altitude().second;

    if(takeoffAlt > 5 || takeoffAlt < 2){
        std::cout << "wrong takeoff alt: " << takeoffAlt << std::endl;
        return;

    }


    setFlightMode(Telemetry::FlightMode::Hold);
    if (m_action->arm() != Action::Result::Success) {
        std::cerr << "Failed to arm.\n";
        return;
    }
    std::cout << "Armed.\n";
    std::cout << "waiting takeoff...\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));  // 10Hz
    std::cout << "Takeoff starting....\n";

    m_action->takeoff_async(nullptr);
    std::string input;

    while(true){
        std::cout << "Enter a command: " << std::endl;
        std::cin >> input;

        if(input == "disarm"){
            std::cout << "disarming..." << std::endl;
            forceDisarm();
            return;

        }

        if(input == "land"){
            std::cout << "landing..." << std::endl;
            m_action->land();
            std::cout << "landing end?" << std::endl;


        }

        if(input == "mission"){


            std::vector<Mission::MissionItem> mission_items;

            Mission::MissionItem wp1{};
            wp1.latitude_deg = -35.365262;
            wp1.longitude_deg = 149.16593;
            wp1.relative_altitude_m = 5.0;
            wp1.speed_m_s = 5.0;

            Mission::MissionItem wp2{};
            wp2.latitude_deg = -35.364800;  // yaklaşık 50 m güney
            wp2.longitude_deg = 149.166200;
            wp2.relative_altitude_m = 5.0;
            wp2.speed_m_s = 5.0;

            mission_items.push_back(wp1);
            mission_items.push_back(wp2);
            Mission::MissionPlan mission_plan{};
            mission_plan.mission_items = mission_items;

            // ✅ Mission yükle
            Mission mission(m_system);
            Mission::Result upload_result = mission.upload_mission(mission_plan);
            if (upload_result != Mission::Result::Success) {
                std::cout << "mission upload failed" << std::endl;

                continue;
            }



            // Mission Başlat
            if (mission.start_mission() != Mission::Result::Success) {
                std::cerr << "Misyon başlatılamadı.\n";
                return;
            }

            std::cout << "Misyon başladı.\n";

        }

    }

}




void FController::forceDisarm()
{
    mavlink_message_t message;
    std::cout << "-----forceDisarm----" << std::endl;

    auto mavlink = std::make_shared<mavsdk::MavlinkPassthrough>(m_system);


    mavlink_command_long_t cmd{};
    cmd.target_system =  mavlink->get_our_sysid();
    cmd.target_component = 1;
    cmd.command = MAV_CMD_COMPONENT_ARM_DISARM;
    cmd.confirmation = 0;
    cmd.param1 = 0;          // 0 = disarm, 1 = arm
    cmd.param2 = 21196;      // Magic value to force disarm even in-flight

    mavlink_msg_command_long_encode(
        m_mavlink->get_our_sysid(),
        m_mavlink->get_our_compid(),
        &message,
        &cmd);


    m_mavlink->send_message(message);

}

void FController::takeoffAsync()
{
    m_action->takeoff_async(nullptr);
}

void FController::execute2WPMissionFromCurrentGPS(double lat, double log, double relAlt, double speed)
{
    std::vector<Mission::MissionItem> mission_items;

    Mission::MissionItem wp1{};
    wp1.latitude_deg = m_currentLat;
    wp1.longitude_deg = m_currentLong;
    wp1.relative_altitude_m = m_currentRelAlt;
    wp1.speed_m_s = 0;

    Mission::MissionItem wp2{};
    wp2.latitude_deg = lat;  // yaklaşık 50 m güney
    wp2.longitude_deg = log;
    wp2.relative_altitude_m = relAlt;
    wp2.speed_m_s = speed;

    mission_items.push_back(wp1);
    mission_items.push_back(wp2);
    Mission::MissionPlan mission_plan{};
    mission_plan.mission_items = mission_items;

    Mission mission(m_system);
    Mission::Result upload_result = mission.upload_mission(mission_plan);
    if (upload_result != Mission::Result::Success) {
        std::cout << "mission upload failed" << std::endl;

        return;
    }



    // Mission Başlat
    if (mission.start_mission() != Mission::Result::Success) {
        std::cout << "mission start failed" << std::endl;
        return;
    }

    std::cout << "mission started" << std::endl;

}


void FController::goToGPSPointWithVelocity(double targetLat, double targetLong, double targetAlt, double speed_perc)
{
    std::cout << "FController -----goToGPSPointWithVelocity----" << std::endl;

    //ensureTakeoffIfNeeded();

    /*Offboard::VelocityNedYaw dummy{};
    dummy.north_m_s = 0;
    dummy.east_m_s = 0;
    dummy.down_m_s = 0;
    dummy.yaw_deg = 0;

    for (int i = 0; i < 10; ++i) {
        m_offboard->set_velocity_ned(dummy);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }*/

    armIfNeeded();
    setFlightMode(mavsdk::Telemetry::FlightMode::Offboard);


    m_offboard->start();

    const double REACHED_THRESHOLD_METERS = 4;

    auto startTime = std::chrono::steady_clock::now();

    while (  !reachedTarget(targetLat, targetLong, targetAlt, REACHED_THRESHOLD_METERS)) {



        double currentLat = m_telemData.latitude;
        double currentLon = m_telemData.longitute;
        double currentAlt = m_telemData.relAltitude;

        double dNorth, dEast;
        calculateDistanceVector(currentLat, currentLon, targetLat, targetLong, dNorth, dEast);
        double dUp = targetAlt - currentAlt;

        double user_speed_percent = std::clamp(speed_perc, 0.0, 100.0);
        double max_speed = 30;  // 100 için
        double min_speed = 3;  // 0 için
        double user_max_speed = min_speed + (max_speed - min_speed) * (user_speed_percent / 100.0);

        // 2. Hedefe olan toplam mesafeyi ölç
        double distance = std::sqrt(dNorth*dNorth + dEast*dEast + dUp*dUp);

        // 3. Mesafe yaklaştıkça yavaşlayan gerçek hız
        double approach_distance = 15.0;
        double final_speed = user_max_speed;
        if (distance < approach_distance) {
            final_speed = min_speed + ( 5 - min_speed) * (distance / approach_distance);
        }

        // Normalize yön vektörü
        double norm = std::sqrt(dNorth * dNorth + dEast * dEast + dUp * dUp);
        double vn = (dNorth / norm) * final_speed;
        double ve = (dEast / norm) * final_speed;
        double vd = -(dUp / norm) * final_speed;


        // Yaw'u hedefe baksın diye isteğe bağlı hesaplıyoruz
        double yaw_deg = atan2(dEast, dNorth) * 180.0 / M_PI;

        Offboard::VelocityNedYaw velocity{};
        velocity.north_m_s = vn;
        velocity.east_m_s = ve;
        velocity.down_m_s = vd;
        velocity.yaw_deg = yaw_deg;

        m_offboard->set_velocity_ned(velocity);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Hover gibi davranmak için dur
    Offboard::VelocityNedYaw stop{};
    stop.north_m_s = 0;
    stop.east_m_s = 0;
    stop.down_m_s = 0;
    stop.yaw_deg = m_telemData.yaw;

    m_offboard->set_velocity_ned(stop);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    m_offboard->stop();
    setFlightMode(mavsdk::Telemetry::FlightMode::Hold);
}




bool FController::setCurrentPositionAsHome()
{
    std::cout << "FController::setCurrentPositionAsHome()..." << std::endl;

    if (m_telemData.satCount < 4)
        return false;

    if (m_telemData.latitude == 0.0f || m_telemData.longitute == 0.0f)
        return false;
    std::cout << "FController::setCurrentPositionAsHome()  1" << std::endl;

    mavlink_message_t msg;
    mavlink_msg_command_long_pack(
        m_mavlink->get_our_sysid(),
        m_mavlink->get_our_compid(),
        &msg,
        m_mavlink->get_target_sysid(),
        m_mavlink->get_target_compid(),
        MAV_CMD_DO_SET_HOME,
        0,
        1,
        0, 0, 0,
        static_cast<double>(m_telemData.latitude),
        static_cast<double>(m_telemData.longitute),
        m_telemData.absAltitude
        );

    m_mavlink->send_message(msg);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));


    return true;
}



void FController::land()
{
    std::cout << "landing..." << std::endl;
    m_action->land();
    std::cout << "landing end" << std::endl;
}




void FController::calculateDistanceVector(double lat1, double lon1, double lat2, double lon2, double& outNorth, double& outEast)
{
    // WGS84 varsayımıyla yaklaşık dönüşüm
    constexpr double R = 6378137.0; // Dünya yarıçapı metre cinsinden

    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    double latRad = lat1 * M_PI / 180.0;

    outNorth = dLat * R;
    outEast = dLon * R * cos(latRad);
}


bool FController::reachedTarget(double targetLat, double targetLon, double targetAlt, double threshold_m)
{
    double currentLat = m_telemData.latitude;
    double currentLon = m_telemData.longitute;
    double currentAlt = m_telemData.relAltitude;

    double dNorth, dEast;
    calculateDistanceVector(currentLat, currentLon, targetLat, targetLon, dNorth, dEast);
    double dAlt = targetAlt - currentAlt;
    //std::cout << "reachedTarget?   dNorth: " << dNorth << ", dEast: " << dEast << std::endl;

    double dist = sqrt(dNorth*dNorth + dEast*dEast + dAlt*dAlt);
    return dist < threshold_m;
}


void FController::testMotor(int motorIndex, float percent, float timeout){
    std::cout << "----- Motor Test ---- Motor: " << motorIndex << " Guc: %" << percent << std::endl;

    //Drone'a göndermek için posta hazırlama
    mavlink_message_t message;
    mavlink_command_long_t cmd{};

    //Postanın gideceği yer
    cmd.target_system = m_mavlink->get_target_sysid();
    cmd.target_component = m_mavlink->get_target_compid();

    //Gönderilen komut
    cmd.command = MAV_CMD_DO_MOTOR_TEST;
    cmd.confirmation = 0;

    cmd.param1 = motorIndex; //Hangi Motor
    cmd.param2 = 0; //Güç Tipi
    cmd.param3 = percent; //Güç Yüzdesi
    cmd.param4 = timeout; //Testin Süreceği Miktar
    cmd.param5 = 1; //Kaç Motor Test Edliecek
    cmd.param6 = 0; //Motor Test Sırası
    cmd.param7 = 0; //Boş

    //Paketle Ve Gönder
    mavlink_msg_command_long_encode(
        m_mavlink->get_our_sysid(),
        m_mavlink->get_our_compid(),
        &message,
        &cmd);

    //Ağa Gönder
    m_mavlink->send_message(message);
}

void FController::testAllMotors(float percent, float timeout){
    std::cout << "----- Motor Test ---- Testing All Motors Pwr: %" << percent << std::endl;
    for(int i = 1; i <= 4; i++){
        testMotor(i, percent, timeout);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}