#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

class Sensor
{
public:
    virtual void showReading(int value) = 0;
};

class Camera : public Sensor
{
public:
    void showReading(int objects)
    {
        cout << "Camera: Objects detected = " << objects << endl;
    }
};

class Radar : public Sensor
{
public:
    void showReading(int distance)
    {
        cout << "Radar: Nearest vehicle distance = " << distance << " m" << endl;
    }
};

class Ultrasonic : public Sensor
{
public:
    void showReading(int distance)
    {
        cout << "Ultrasonic: Obstacle distance = " << distance << " m" << endl;
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

    string analyze(int speed, int radarDistance, int ultrasonicDistance, int cameraObjects)
    {
        int score = 0;
        if (radarDistance < 20)
            score += 3;
        else if (radarDistance < 40)
            score += 2;
        if (ultrasonicDistance < 10)
            score += 3;
        else if (ultrasonicDistance < 20)
            score += 2;
        if (speed > 80)
            score += 3;
        else if (speed > 50)
            score += 1;
        if (cameraObjects >= 3)
            score += 2;
        else if (cameraObjects >= 1)
            score += 1;
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
    string name, specialization, phone, location, showroomAddress;

public:
    Technician(int i, string n, int a, int exp, string skill, string ph, string loc, string address)
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

    void displayDetails()
    {
        cout << "\nTechnician ID     : " << id << endl;
        cout << "Name              : " << name << endl;
        cout << "Age               : " << age << endl;
        cout << "Experience        : " << experience << " years" << endl;
        cout << "Specialization    : " << specialization << endl;
        cout << "Mobile number     : " << phone << endl;
        cout << "Showroom location : " << location << endl;
        cout << "Showroom address  : " << showroomAddress << endl;
    }
};

void showDriverName(string driverName)
{
    cout << "Driver: " << driverName << endl;
}

void adjustSpeedForSafety(int &recommendedSpeed, string risk)
{
    if (risk == "HIGH")
        recommendedSpeed = 0;
    else if (risk == "MEDIUM" && recommendedSpeed > 50)
        recommendedSpeed = 50;
}

int findNextTechnician(Technician technicians[], int totalTechnicians, string selectedLocation, int startIndex)
{
    for (int i = startIndex; i < totalTechnicians; i++)
    {
        if (technicians[i].getLocation() == selectedLocation)
            return i;
    }
    return -1;
}

int main()
{
    srand((unsigned int)time(0));
    string driverName;

    cout << "========================================\n";
    cout << "   SMART VEHICLE ASSISTANCE SYSTEM\n";
    cout << "========================================\n";
    cout << "Enter driver name: ";
    getline(cin, driverName);
    showDriverName(driverName);

    Camera camera;
    Radar radar;
    Ultrasonic ultrasonic;
    Sensor *sensors[3] = {&camera, &radar, &ultrasonic};
    ADASDecision decision;

    Technician technicians[10] = {
        Technician(1, "Sujith", 28, 5, "Camera and ADAS", "9000000001", "Coimbatore", "12, Rajiv Nagar, Gandhipuram, Coimbatore"),
        Technician(2, "Nandhini", 30, 7, "Radar and ADAS", "9000000002", "Coimbatore", "13, Surya Nagar, Gandhipuram, Coimbatore"),
        Technician(3, "Vani", 27, 4, "Ultrasonic sensors", "9000000003", "Coimbatore", "14, ABC Apartment, Gandhipuram, Coimbatore"),
        Technician(4, "Arun", 32, 8, "Vehicle diagnostics", "9000000004", "Coimbatore", "15, DEF Apartment, Gandhipuram, Coimbatore"),
        Technician(5, "Kavin", 29, 6, "ADAS calibration", "9000000005", "Coimbatore", "16, GHI Avenue, Gandhipuram, Coimbatore"),
        Technician(6, "Priya", 28, 5, "Camera and ADAS", "9000000006", "Tiruppur", "17, LIC Colony, Avinashi Road, Tiruppur"),
        Technician(7, "Ravi", 34, 9, "Radar and ADAS", "9000000007", "Tiruppur", "18, Gandhi Nagar, Avinashi Road, Tiruppur"),
        Technician(8, "Meena", 26, 3, "Ultrasonic sensors", "9000000008", "Tiruppur", "19, Ganesh Nagar, Avinashi Road, Tiruppur"),
        Technician(9, "Karthik", 31, 7, "Vehicle diagnostics", "9000000009", "Tiruppur", "20, Hindusthan Nagar, Avinashi Road, Tiruppur"),
        Technician(10, "Divya", 29, 6, "ADAS calibration", "9000000010", "Tiruppur", "21, Vani Colony, Avinashi Road, Tiruppur")
    };

    const int totalTechnicians = 10;
    char continueMonitoring = 'Y';

    while (continueMonitoring == 'Y' || continueMonitoring == 'y')
    {
      cout << "\n---------- LIVE SENSOR MONITORING ----------\n";
      bool assistancePrompted = false;
      while (!assistancePrompted)
      {
        int cameraObjects = rand() % 5;
        int radarDistance = 10 + rand() % 91;
        int ultrasonicDistance = 5 + rand() % 46;
        int vehicleSpeed = rand() % 101;
        cout << "\n--------------------------------------------\n";
        sensors[0]->showReading(cameraObjects);
        sensors[1]->showReading(radarDistance);
        sensors[2]->showReading(ultrasonicDistance);
        cout << "Vehicle speed: " << vehicleSpeed << " km/h" << endl;
        string risk = decision.analyze(vehicleSpeed, radarDistance, ultrasonicDistance, cameraObjects);
        cout << "Risk category: " << risk << endl;
        int recommendedSpeed = vehicleSpeed;
        adjustSpeedForSafety(recommendedSpeed, risk);
        cout << "Illustrative safe-speed recommendation: " << recommendedSpeed << " km/h" << endl;

        if (risk == "HIGH")
        {
          assistancePrompted = true;
          cout << "\n*** HIGH RISK DETECTED! ***\n";
          cout << "Monitoring paused.\n";
          char needHelp;
          cout << "Do you need vehicle assistance? (Y/N): ";
          cin >> needHelp;

          if (needHelp == 'Y' || needHelp == 'y')
          {
            int locationChoice;
            cout << "\nSelect your location:\n1. Coimbatore\n2. Tiruppur\n";
            cout << "Enter choice (1 or 2): ";
            cin >> locationChoice;

            string selectedLocation;
            if (locationChoice == 1)
                selectedLocation = "Coimbatore";
            else if (locationChoice == 2)
                selectedLocation = "Tiruppur";
            else
                selectedLocation = "";

               if (selectedLocation != "")
               {
                  cout << "\nTechnicians in " << selectedLocation << ":\n";
                  int startIndex = 0, shown = 0;
                  char satisfied = 'N';

                  while (shown < 5 && satisfied != 'Y' && satisfied != 'y')
                  {
                    int foundIndex = findNextTechnician(technicians, totalTechnicians, selectedLocation, startIndex);
                    if (foundIndex == -1)
                        break;

                    technicians[foundIndex].displayDetails();
                    shown++;
                    startIndex = foundIndex + 1;

                    cout << "\nSatisfied with this technician? (Y/N): ";
                    cin >> satisfied;

                    if (satisfied == 'Y' || satisfied == 'y')
                        cout << "Please contact the technician using the displayed number.\n";
                    else if (shown < 5)
                        cout << "Showing the next technician...\n";
                  }

                    if (shown == 5 && satisfied != 'Y' && satisfied != 'y')
                        cout << "No more sample technicians listed for this location.\n";
               }
                    else
                        cout << "Invalid location choice.\n";
            }
                else
                    cout << "No assistance selected. Drive safely.\n";
        }
            else
                Sleep(700);
    }

        cout << "\nResume live monitoring? (Y/N): ";
        cin >> continueMonitoring;
    }

    cout << "\nSmart Vehicle Assistance System closed.\n";
    return 0;
}
