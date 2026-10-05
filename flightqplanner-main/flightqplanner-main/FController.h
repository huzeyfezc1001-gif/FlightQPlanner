#ifndef FCONTROLLER_H
#define FCONTROLLER_H

#include <chrono>
#include <mavsdk/mavsdk.hpp>
#include <mavsdk/plugins/action/action.hpp>
#include <mavsdk/plugins/telemetry/telemetry.hpp>
#include <mavsdk/plugins/param/param.hpp>
#include <mavsdk/plugins/offboard/offboard.hpp>
#include <mavsdk/mavlink/protocol.h>
#include <mavsdk/mavlink/common/mavlink.h>
#include <mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.hpp>
#include <mavsdk/plugins/offboard/offboard.hpp>


#include <atomic>
#include <memory>
#include <string>
#include <thread>

#include <QObject>



class TelemData{
public:
    float latitude;
    float longitute;
    float relAltitude;
    float absAltitude;
    float yaw;
    float roll;
    float pitch;
    int satCount;
};


class FController :  public QObject{
    Q_OBJECT
public:

    explicit FController(bool isSimulation);
    ~FController();

    bool connect();

    bool armIfNeeded();
    bool isArmed();
    float getTakeoffAltitude();
    float setTakeoffAltitude(float alt);

    void forceDisarm();
    void takeoffAsync();
    void execute2WPMissionFromCurrentGPS(double lat, double log, double relAlt, double speed);
    void goToGPSPointWithVelocity(double targetLat, double targetLong, double targetAlt, double speed_perc);
    void land();

    void test0_async();
    bool setCurrentPositionAsHome();
    void testAllMotors(float percent, float timeout);
    void testMotor(int motorIndex, float percent, float timeout);

    std::shared_ptr<mavsdk::System> getSystem() {
        return m_system;
    }




private:

    bool setInitialParameters();
    bool setFlightMode(mavsdk::Telemetry::FlightMode mode);

    void subscribeTelemetry();
    void pushGPSStat(mavsdk::Telemetry::GpsInfo gps_info);
    void pushPosition(mavsdk::Telemetry::Position pos);
    void pushYPR(mavsdk::Telemetry::EulerAngle eulerAngle);

    bool reachedTarget(double targetLat, double targetLon, double targetAlt, double threshold_m);
    void calculateDistanceVector(double lat1, double lon1, double lat2, double lon2, double& outNorth, double& outEast);

    TelemData m_telemData;




    std::atomic<bool> keep_running{false};
    std::unique_ptr<std::thread> flow_thread;
    bool m_gps_available = false;

    std::string m_connectionUrl;
    std::unique_ptr<mavsdk::Mavsdk> m_mavsdk;
    std::shared_ptr<mavsdk::MavlinkPassthrough> m_mavlink;
    std::shared_ptr<mavsdk::System> m_system;
    std::shared_ptr<mavsdk::Action> m_action;
    std::shared_ptr<mavsdk::Telemetry> m_telemetry;
    std::shared_ptr<mavsdk::Param> m_param;
    std::shared_ptr<mavsdk::Offboard> m_offboard;

    double m_lastAccel = 0;


    bool m_isSim = false;
    double m_currentLat, m_currentLong, m_currentRelAlt;



signals:
    void sgn_pushGPSData(double lat, double log, double absAlt, double relAlt);
    void sgn_pushGPSInfo(int satCnt);
    void sgn_pushYPR(double y, double p, double r);


};

#endif // FCONTROLLER_H
