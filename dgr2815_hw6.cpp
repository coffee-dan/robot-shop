#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// /////////////////////////////////////
//        R O B O T   P A R T
// /////////////////////////////////////
class RobotPart {
  public:
    RobotPart(string n, int m, double c, string d, string i)
      : name(n), model_number(m), cost(c), description(d), image_filename(i) { }
    string dtos(double d, int nd);
    string to_string();
    virtual string part_to_string()=0;
  protected:
    string name;
    int model_number;
    double cost;
    string description;
    string image_filename;
};

string RobotPart::dtos(double d, int nd) {
  stringstream ss;
  ss << fixed << setprecision(nd) << d;
  return ss.str();
}
string RobotPart::to_string() {
  return name+" - "+std::to_string(model_number)+" $"+dtos(cost, 2)+"\n"+description+"\n"+image_filename;
}
// /////////////////////////////////////
//              A R M
// /////////////////////////////////////
class Arm : public RobotPart {
  public:
    Arm(string n, int m, double c, string d, string i, double m_p)
      : RobotPart(n, m, c, d, i), max_power(m_p) { }
    string part_to_string();
  private:
    double max_power;
};

string Arm::part_to_string() {
  string output = RobotPart::to_string();
  return "--Arm--\n"+output + "\nMax Power : "+dtos(max_power, 2)+"\n";
}
// /////////////////////////////////////
//          B A T T E R Y
// /////////////////////////////////////
class Battery : public RobotPart {
  public:
    Battery(string n, int m, double c, string d, string i, double p, double m_e)
      : RobotPart(n, m, c, d, i), power_available(p), max_energy(m_e) { }
    string part_to_string();
  private:
    double power_available;
    double max_energy;
};

string Battery::part_to_string() {
  string output = RobotPart::to_string();
  return "--Battery--\n"+output +"\nPower Available : "+std::to_string(power_available)+"\nMax Energy : "+dtos(max_energy, 2)+"\n";
}
// /////////////////////////////////////
//             H E A D
// /////////////////////////////////////
class Head : public RobotPart {
  public:
    Head(string n, int m, double c, string d, string i, double p)
      :  RobotPart(n, m, c, d, i), power(p) { }
    string part_to_string();
  private:
    double power;
};

string Head::part_to_string() {
  string output = RobotPart::to_string();
  return "--Head--\n"+output +"\nPower : "+dtos(power, 2)+"\n";
}
// /////////////////////////////////////
//         L O C O M O T O R
// /////////////////////////////////////
class Locomotor : public RobotPart {
  public:
    Locomotor(string n, int m, double c, string d, string i, double m_p)
      : RobotPart(n, m, c, d, i), max_power(m_p) { }
    string part_to_string();
  private:
    double max_power;
};

string Locomotor::part_to_string() {
  string output = RobotPart::to_string();
  return "--Locomotor--\n"+output +"\nMax Power : "+dtos(max_power, 2)+"\n";
}
// /////////////////////////////////////
//            T O R S O
// /////////////////////////////////////
class Torso : public RobotPart {
  public:
    Torso(string n, int m, double c, string d, string i, int b, int m_a)
      :  RobotPart(n, m, c, d, i), battery_compartments(b), max_arms(m_a) { }
    string part_to_string();
  private:
    int battery_compartments;
    int max_arms;
};

string Torso::part_to_string() {
  string output = RobotPart::to_string();
  return "--Torso--\n"+output +"\nBattery Compartments : "+std::to_string(battery_compartments)+"\nMax Arms : "+std::to_string(max_arms)+"\n";
}
// /////////////////////////////////////
//         R O B O T   M O D E L
// /////////////////////////////////////
class RobotModel {
  public:
    double cost();
    double max_speed();
    double max_battery_life();
  private:
    string name;
    int model_number;
    RobotPart* torso;
    RobotPart* head;
    RobotPart* locomotor;
    RobotPart* arm;
    RobotPart* battery;
};
// /////////////////////////////////////
//              V I E W
// /////////////////////////////////////
class View {
  public:
    View(vector<RobotPart*>& rps) : robotparts(rps) { }
    string get_menu();
    string get_part_list();
    string get_help();
  private:
    vector<RobotPart*>& robotparts;
};
// /////////////////////////////////////
//         C O N T R O L L E R
// /////////////////////////////////////
class Controller {
  public:
    Controller(vector<RobotPart*>& rps) : robotparts(rps) { }
    void cli();
    void execute_cmd(int cmd);
  private:
    double get_double(string prompt, double max_double);
    int get_int(string prompt, int max_int);
    int get_int(string prompt, int min_int, int max_int);
    string get_string(string prompt);
    vector<RobotPart*>& robotparts;
};

double Controller::get_double(string prompt, double max_double) {
  double result;
  while(true) {
    cout << prompt;
    cin >> result;
    cin.ignore(); // consume \n
    if (0 <= result && result <= max_double) break;
    cout << "Please enter an integer between 0 and " << max_double << endl;
  }
  return result;
}
int Controller::get_int(string prompt, int max_int) {
  int result;
  while(true) {
    cout << prompt;
    cin >> result;
    cin.ignore(); // consume \n
    if (0 <= result && result <= max_int) break;
    cout << "Please enter an integer between 0 and " << max_int << endl;
  }
  return result;
}
int Controller::get_int(string prompt, int min_int, int max_int) {
  int result;
  while(true) {
    cout << prompt;
    cin >> result;
    cin.ignore(); // consume \n
    if (min_int <= result && result <= max_int) break;
    cout << "Please enter an integer between "<< min_int << " and " << max_int << endl;
  }
  return result;
}
string Controller::get_string(string prompt) {
  string result;
  cout << prompt;
  getline(cin, result);
  return result;
}

void Controller::cli() {
  int cmd = -1;
  string prompt = "\nWhich part would you like to create?\nArm : 1\nBattery : 2\nHead : 3\nLocomotor : 4\nTorso : 5\n\n";
  while(cmd != 0) {
    cmd = get_int(prompt, 5);
    execute_cmd(cmd);
  }
  cout << "Goodbye\n";
}
void Controller::execute_cmd(int cmd) {
  if(cmd == 0) return;
  cout << "Gathering default robot part information...\n";
  string name, description, image_filename;
  int model_number;
  double cost;

  name = get_string("Name? ");
  model_number = get_int("Model Number? ", 10000);
  cost = get_double("Cost[$]? ", 10000.0);
  description = get_string("Description? ");
  image_filename = "image.png";

  RobotPart* part;

  if(cmd == 1) {
    cout << "Gathering unique robot arm information...\n";
    double max_power;

    max_power = get_double("Max Power[W]? ", 9000);
    part = new Arm{name, model_number, cost, description, image_filename, max_power};
    robotparts.push_back(part);

  } else if(cmd == 2) {
    cout << "Gathering unique battery information...\n";
    double power_available, max_energy;

    power_available = get_double("Max Power[W]? ", 90000);
    max_energy = get_double("Max Energy[kWh]? ", 10000);
    part = new Battery{name, model_number, cost, description, image_filename, power_available, max_energy};
    robotparts.push_back(part);

  } else if(cmd == 3) {
    cout << "Gathering unique robot head information...\n";
    double power;

    power = get_double("Power? ", 10000);
    part = new Head{name, model_number, cost, description, image_filename, power};
    robotparts.push_back(part);

  } else if(cmd == 4) {
    cout << "Gathering unique locomotor information...\n";
    double max_power;

    max_power = get_double("Max Power[W]? ", 9000);
    part = new Locomotor{name, model_number, cost, description, image_filename, max_power};
    robotparts.push_back(part);

  } else if(cmd == 5) {
    cout << "Gathering unique robot torso information...\n";
    int battery_compartments, max_arms;

    battery_compartments = get_int("Battery Compartments? ", 1, 3);
    max_arms = get_int("Max Arms? ", 2);
    part = new Torso{name, model_number, cost, description, image_filename, battery_compartments, max_arms};
    robotparts.push_back(part);

  }
}

int main() {
  vector<RobotPart*> rps;
  Controller controller{rps};
  controller.cli();
}
