#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

struct ServiceRequest
{
    string service;
    int priority;
    int arrivalOrder;
};

class PriorityQueue
{
private:
    ServiceRequest requests[10];
    int count;
    int nextOrder;

public:
    PriorityQueue()
    {
        count = 0;
        nextOrder = 1;
    }

    bool isEmpty()
    {
        return count == 0;
    }

    void enqueue(string service, int priority)
    {
        ServiceRequest newRequest;
        newRequest.service = service;
        newRequest.priority = priority;
        newRequest.arrivalOrder = nextOrder++;

        int i = count - 1;
        while (i >= 0 && requests[i].priority > newRequest.priority)
        {
            requests[i + 1] = requests[i];
            i--;
        }

        requests[i + 1] = newRequest;
        count++;
        cout << service << " service request queued with priority "
             << priority << ".\n";
    }

    ServiceRequest dequeue()
    {
        ServiceRequest first = requests[0];
        for (int i = 0; i < count - 1; i++)
            requests[i] = requests[i + 1];
        count--;
        return first;
    }

    void displayPending()
    {
        if (isEmpty())
        {
            cout << "Pending service requests: 0 (None)\n";
            return;
        }

        cout << "\n---------- PENDING SERVICE REQUESTS ----------\n";
        for (int i = 0; i < count; i++)
        {
            cout << i + 1 << ". Service: " << requests[i].service
                 << " | Priority: " << requests[i].priority
                 << " | Arrival order: " << requests[i].arrivalOrder;

            if (requests[i].priority == 1)
                cout << " (CRITICAL)";
            else if (requests[i].priority == 2)
                cout << " (HIGH)";
            else
                cout << " (NORMAL)";

            cout << endl;
        }
    }
};

class Sensor
{
public:
    virtual void showReading(int value) = 0;
};

class Camera : public Sensor
{
public:
    void showReading(int value)
    {
        cout << "Camera Objects       : " << value << endl;
    }
};

class Radar : public Sensor
{
public:
    void showReading(int value)
    {
        cout << "Radar Distance       : " << value << " cm" << endl;
    }
};

class Ultrasonic : public Sensor
{
public:
    void showReading(int value)
    {
        cout << "Ultrasonic Distance  : " << value << " cm" << endl;
    }
};


class ADASDecision
{
public:
    string analyze(int speed)
    {
        if (speed > 80)
            return "HIGH";
        else if (speed > 50)
            return "MEDIUM";
        return "NORMAL";
    }


    string analyze(int speed, int radarDistance,
                   int ultrasonicDistance, int objects)
    {
        int score = 0;

        if (radarDistance >= 0)
        {
            if (radarDistance < 20)
                score += 3;
            else if (radarDistance < 40)
                score += 2;
        }

        if (ultrasonicDistance >= 0)
        {
            if (ultrasonicDistance < 10)
                score += 3;
            else if (ultrasonicDistance < 20)
                score += 2;
        }

        if (speed > 80)
            score += 3;
        else if (speed > 50)
            score += 1;

        if (objects >= 0)
        {
            if (objects >= 3)
                score += 2;
            else if (objects >= 1)
                score += 1;
        }

        if (score >= 6)
            return "HIGH";
        else if (score >= 3)
            return "MEDIUM";
        return "NORMAL";
    }
};

class Technician
{
private:
    int id, age, experience;
    string name, specialization, phone;
    string location, showroomAddress;

public:
    Technician(int i, string n, int a, int exp, string skill,
               string ph, string loc, string address)
    {
        id = i;
        name = n;
        age = a;
        experience = exp;
        specialization = skill;
        phone = ph;
        location = loc;
        showroomAddress = address;
    }

    string getLocation()
    {
        return location;
    }

    string getSpecialization()
    {
        return specialization;
    }

    void displayDetails()
    {
        cout << "\n--------------------------------------------\n";
        cout << "Technician ID       : " << id << endl;
        cout << "Name                : " << name << endl;
        cout << "Age                 : " << age << endl;
        cout << "Experience          : " << experience << " years\n";
        cout << "Specialization      : " << specialization << endl;
        cout << "Mobile Number       : " << phone << endl;
        cout << "Showroom Location   : " << location << endl;
        cout << "Showroom Address    : " << showroomAddress << endl;
        cout << "--------------------------------------------\n";
    }
};

int findNextTechnician(Technician technicians[], int total,
                       string location, string service, int startIndex)
{
    for (int i = startIndex; i < total; i++)
    {
        if (technicians[i].getLocation() == location &&
            technicians[i].getSpecialization().find(service)
                != string::npos)
        {
            return i;
        }
    }

    return -1;
}

bool findTechniciansForRequest(Technician technicians[], int total,
                               string location, string service)
{
    cout << "\nMatching " << service << " technicians in "
         << location << ":\n";

    int startIndex = 0;
    int shown = 0;
    char satisfied = 'N';

    while (satisfied != 'Y' && satisfied != 'y')
    {
        int index = findNextTechnician(
            technicians, total, location, service, startIndex);

        if (index == -1)
        {
            if (shown == 0)
                cout << "No matching technicians found in this location.\n";
            else
                cout << "No more matching technicians available.\n";
            return false;
        }

        technicians[index].displayDetails();
        shown++;
        startIndex = index + 1;

        cout << "Satisfied with this technician? (Y/N): ";
        cin >> satisfied;

        if (satisfied == 'Y' || satisfied == 'y')
            cout << "Please contact the technician using the displayed number.\n";
        else
            cout << "Searching for the next matching technician...\n";
    }

    return true;
}

int assignPriority(string sensorName)
{
    if (sensorName == "Radar")
        return 1;
    if (sensorName == "Camera")
        return 2;
    return 3;
}

void generateValues(int accSpeed, int &actualSpeed, bool &overrideDetected,
                    int &objects, int &radarDistance, int &ultrasonicDistance)
{
    overrideDetected = (rand() % 3 == 0);

    if (overrideDetected)
    {
        actualSpeed = accSpeed + 5 + rand() % 16;
        if (actualSpeed > 150)
            actualSpeed = 150;
    }
    else
    {
        actualSpeed = rand() % (accSpeed + 1);
    }

    objects = rand() % 5;
    radarDistance = 10 + rand() % 91;
    ultrasonicDistance = 5 + rand() % 146;
}

void displayCycle(int cycle, string driverName, int accSpeed,
                  int actualSpeed, bool overrideDetected,
                  int objects, int radarDistance, int ultrasonicDistance,
                  bool cameraFault, bool radarFault, bool ultrasonicFault,
                  string risk, Sensor *sensors[])
{
    bool objectNear =
        (!radarFault && radarDistance < 20) ||
        (!ultrasonicFault && ultrasonicDistance < 100);

    cout << "\n\n============================================\n";
    cout << "Cycle                : " << cycle << endl;
    cout << "Driver Name          : " << driverName << endl;
    cout << "ACC Set Speed        : " << accSpeed << " km/h\n";
    cout << "Actual Vehicle Speed : " << actualSpeed << " km/h\n";

    cout << "\n--------------- SENSOR READINGS ------------\n";

    if (cameraFault)
        cout << "Camera               : INVALID\n";
    else
        sensors[0]->showReading(objects);

    if (radarFault)
        cout << "Radar                : INVALID\n";
    else
        sensors[1]->showReading(radarDistance);

    if (ultrasonicFault)
        cout << "Ultrasonic           : INVALID\n";
    else
        sensors[2]->showReading(ultrasonicDistance);

    cout << "Confirmed Faults     : Camera=" << (cameraFault ? "FAULT" : "OK")
         << ", Radar=" << (radarFault ? "FAULT" : "OK")
         << ", Ultrasonic=" << (ultrasonicFault ? "FAULT" : "OK") << endl;

    cout << "\n--------------- ACC MONITORING -------------\n";
    cout << "Road Risk            : " << risk << endl;
    cout << "Driver Override      : "
         << (overrideDetected ? "DETECTED" : "NOT DETECTED") << endl;


    if (risk == "HIGH")
        cout << "CRITICAL WARNING: HIGH ROAD RISK. Stop safely.\n";
    else if (risk == "MEDIUM")
        cout << "WARNING: MEDIUM ROAD RISK. Reduce speed and maintain safe distance.\n";
    else
        cout << "No immediate hazard detected. Continue driving safely.\n";

    if (overrideDetected && objectNear)
        cout << "WARNING: Override detected while an object is nearby!\n";
    else if (overrideDetected)
        cout << "WARNING: Actual speed exceeds ACC set speed.\n";
}

int main()
{
    srand((unsigned int)time(0));

    string driverName;
    int accSpeed;

    cout << "============================================\n";
    cout << "     SMART VEHICLE ASSISTANCE SYSTEM\n";
    cout << "============================================\n";

    cout << "Enter driver name: ";
    getline(cin, driverName);

    cout << "Enter ACC set speed (1-120 km/h): ";
    cin >> accSpeed;

    if (accSpeed < 1 || accSpeed > 120)
    {
        cout << "Invalid ACC speed.\n";
        return 0;
    }

    Technician technicians[6] = {
        Technician(1, "Sujith", 28, 5, "Camera and ADAS",
                   "9000000001", "Coimbatore",
                   "12, Rajiv Nagar, Gandhipuram, Coimbatore"),

        Technician(2, "Nandhini", 30, 7, "Radar and ADAS",
                   "9000000002", "Coimbatore",
                   "13, Surya Nagar, Gandhipuram, Coimbatore"),

        Technician(3, "Priya", 28, 5, "Camera and Ultrasonic and ADAS",
                   "9000000003", "Tiruppur",
                   "17, LIC Colony, Avinashi Road, Tiruppur"),

        Technician(4, "Ravi", 34, 9, "Radar and ADAS",
                   "9000000004", "Tiruppur",
                   "18, Gandhi Nagar, Avinashi Road, Tiruppur"),

        Technician(5, "Meena", 29, 6, "Camera and Ultrasonic and ADAS",
                   "9000000005", "Tiruppur",
                   "19, Ganesh Nagar, Avinashi Road, Tiruppur"),

        Technician(6, "Arun", 32, 8, "Camera and ADAS",
                   "9000000006", "Coimbatore",
                   "15, DEF Apartment, Gandhipuram, Coimbatore")
    };

    Camera camera;
    Radar radar;
    Ultrasonic ultrasonic;

    Sensor *sensors[3] = {&camera, &radar, &ultrasonic};
    ADASDecision decision;

    string targets[3] = {"NORMAL", "MEDIUM", "HIGH"};
    char continueMonitoring = 'Y';

    while (continueMonitoring == 'Y' || continueMonitoring == 'y')
    {
        PriorityQueue serviceQueue;

        bool cameraFault = false;
        bool radarFault = false;
        bool ultrasonicFault = false;


        for (int cycle = 1; cycle <= 7; cycle++)
        {
            int actualSpeed, objects, radarDistance, ultrasonicDistance;
            bool overrideDetected;
            string risk;

            if (cycle <= 3)
            {

                do
                {
                    generateValues(accSpeed, actualSpeed, overrideDetected,
                                   objects, radarDistance, ultrasonicDistance);
                    risk = decision.analyze(actualSpeed, radarDistance,
                                            ultrasonicDistance, objects);
                } while (risk != targets[cycle - 1]);
            }
            else
            {
                generateValues(accSpeed, actualSpeed, overrideDetected,
                               objects, radarDistance, ultrasonicDistance);


                if (cycle == 4) ultrasonicFault = true;
                if (cycle == 5) cameraFault = true;
                if (cycle == 7) radarFault = true;

                int invalidCount = (radarFault ? 1 : 0) + (cameraFault ? 1 : 0) +
                                   (ultrasonicFault ? 1 : 0);

                if (invalidCount == 3)
                    risk = decision.analyze(actualSpeed);
                else
                    risk = decision.analyze(actualSpeed,
                                            radarFault ? -1 : radarDistance,
                                            ultrasonicFault ? -1 : ultrasonicDistance,
                                            cameraFault ? -1 : objects);
            }

            displayCycle(cycle, driverName, accSpeed, actualSpeed, overrideDetected,
                         objects, radarDistance, ultrasonicDistance,
                         cameraFault, radarFault, ultrasonicFault, risk, sensors);


            if (cycle == 4)
            {
                cout << "\nUltrasonic: INVALID reading - FAULT DETECTED\n";
                serviceQueue.enqueue("Ultrasonic", assignPriority("Ultrasonic"));
            }
            else if (cycle == 5)
            {
                cout << "\nCamera: INVALID reading - FAULT DETECTED\n";
                cout << "Ultrasonic fault retained until serviced.\n";
                serviceQueue.enqueue("Camera", assignPriority("Camera"));
            }
            else if (cycle == 6)
            {
                cout << "\nNo new fault. Ultrasonic and Camera faults retained until serviced.\n";
            }
            else if (cycle == 7)
            {
                cout << "\nRadar: INVALID reading - FAULT DETECTED\n";
                cout << "Ultrasonic and Camera faults retained until serviced.\n";
                serviceQueue.enqueue("Radar", assignPriority("Radar"));
            }

            if (cycle >= 4)
                serviceQueue.displayPending();

            Sleep(2000);
        }


        cout << "\n\n========== SERVICE: PRIORITY QUEUE DISPATCH ==========\n";
        cout << "Select your location:\n";
        cout << "1. Coimbatore\n";
        cout << "2. Tiruppur\n";
        cout << "Enter choice (1 or 2): ";

        int locationChoice;
        cin >> locationChoice;

        string location;
        if (locationChoice == 1)
            location = "Coimbatore";
        else if (locationChoice == 2)
            location = "Tiruppur";
        else
        {
            cout << "Invalid location. Service cancelled.\n";
            return 0;
        }


        while (!serviceQueue.isEmpty())
        {
            ServiceRequest request = serviceQueue.dequeue();

            cout << "\n---------- DISPATCHING REQUEST ----------\n";
            cout << "Service             : " << request.service << endl;
            cout << "Priority            : " << request.priority << endl;
            cout << "Arrival order       : " << request.arrivalOrder << endl;

            bool repaired = findTechniciansForRequest(
                technicians, 6, location, request.service);

            if (repaired)
                cout << request.service << " fault serviced.\n";
            else
                cout << request.service << " fault remains active.\n";

            serviceQueue.displayPending();
        }

        cout << "\nAll service requests completed.\n";
        cout << "\nDo you want to continue monitoring? (Y/N): ";
        cin >> continueMonitoring;
    }

    cout << "\nMonitoring stopped. Drive safely!\n";
    return 0;
}
