#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

string dtos(double num, int precision) {
  stringstream ss;
  ss << fixed << setprecision(precision) << num;
  return ss.str();
}
// /////////////////////////////////////
//        R O B O T   P A R T
// /////////////////////////////////////
class RobotPart {
  public:
    RobotPart(string t, string n, int m, double c, double w, string d, string i)
      : type(t), name(n), model_number(m), cost(c), weight(w), description(d), image_filename(i) { }
    string getType() { return type; }
    string getName() { return name; }
    double getCost() { return cost; }
    double getWeight() { return weight; }

    string to_string();
    virtual string part_to_string()=0;
  protected:
    string type;
    string name;
    int model_number;
    double cost;
    double weight;
    string description;
    string image_filename;
};
string RobotPart::to_string() {
  string output = name+" - $"+dtos(cost, 2)+"\nModel #"+std::to_string(model_number)+" - "+image_filename;
  output += "\nDescription : "+description+"\nWeight : "+dtos(weight, 2)+" [Lbs]\n";
  return output;
}
// /////////////////////////////////////
//              A R M
// /////////////////////////////////////
class Arm : public RobotPart {
  public:
    Arm(string t, string n, int m, double c, double w, string d, string i, double m_p)
      : RobotPart(t, n, m, c, w, d, i), max_power(m_p) { }
    double getMaxPower() { return max_power; }
    string part_to_string();
  private:
    double max_power;
};
string Arm::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output+"\nMax Power : "+dtos(max_power, 2)+" [W]\n\n";
}
// /////////////////////////////////////
//          B A T T E R Y
// /////////////////////////////////////
class Battery : public RobotPart {
  public:
    Battery(string t, string n, int m, double c, double w, string d, string i, double p, double m_e)
      : RobotPart(t, n, m, c, w, d, i), power_available(p), max_energy(m_e) { }
    double getPowerAvailable() { return power_available; }
    double getMaxEnergy() { return max_energy; }
    string part_to_string();
  private:
    double power_available;
    double max_energy;
};
string Battery::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"\nPower Available : "+std::to_string(power_available)+" [W]\nMax Energy : "+dtos(max_energy, 2)+" [kWh]\n\n";
}
// /////////////////////////////////////
//             H E A D
// /////////////////////////////////////
class Head : public RobotPart {
  public:
    Head(string t, string n, int m, double c, double w, string d, string i, double p)
      :  RobotPart(t, n, m, c, w, d, i), power(p) { }
    double getPower() { return power; }
    string part_to_string();
  private:
    double power;
};
string Head::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"\nPower : "+dtos(power, 2)+" [W]\n\n";
}
// /////////////////////////////////////
//         L O C O M O T O R
// /////////////////////////////////////
class Locomotor : public RobotPart {
  public:
    Locomotor(string t, string n, int m, double c, double w, string d, string i, double m_p)
      : RobotPart(t, n, m, c, w, d, i), max_power(m_p) { }
    double getMaxPower() { return max_power; }
    string part_to_string();
  private:
    double max_power;
};
string Locomotor::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"\nMax Power : "+dtos(max_power, 2)+" [W]\n\n";
}
// /////////////////////////////////////
//            T O R S O
// /////////////////////////////////////
class Torso : public RobotPart {
  public:
    Torso(string t, string n, int m, double c, double w, string d, string i, int b, int m_a)
      :  RobotPart(t, n, m, c, w, d, i), battery_compartments(b), max_arms(m_a) { }
    int getBatteryCompartments() { return battery_compartments; }
    int getMaxArms() { return max_arms; }

    string part_to_string();
  private:
    int battery_compartments;
    int max_arms;
};
string Torso::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"\nBattery Compartments : "+std::to_string(battery_compartments)+"\nMax Arms : "+std::to_string(max_arms)+"\n\n";
}
// /////////////////////////////////////
//         R O B O T   M O D E L
// /////////////////////////////////////
class RobotModel {
  public:
    RobotModel(string n, int m, RobotPart* t, RobotPart* h, RobotPart* l, RobotPart* a, RobotPart* b, int numOfB, int numOfA)
      :  name(n), model_number(m), torso(t), head(h), locomotor(l), arm(a), battery(b), num_of_batteries(numOfB), num_of_arms(numOfA) { }
    RobotPart* getTorso() { return torso; }
    RobotPart* getHead() { return head; }
    RobotPart* getLocomotor() { return locomotor; }
    RobotPart* getArm() { return arm; }
    RobotPart* getBattery() { return battery; }
    double cost();
    double max_speed();
    double max_battery_life();
    string to_string();
  private:
    string name;
    int model_number;
    RobotPart* torso;
    RobotPart* head;
    RobotPart* locomotor;
    RobotPart* arm;
    RobotPart* battery;
    int num_of_batteries;
    int num_of_arms;
};

double RobotModel::cost() {
  double cost = torso->getCost() + head->getCost();
  cost += locomotor->getCost() + (num_of_arms * arm->getCost());
  cost += (num_of_batteries * battery->getCost());
  return cost;
}
string RobotModel::to_string() {
  string output = "Model : "+name+" - $"+dtos(cost(), 2);
  output += "\nModel #"+std::to_string(model_number)+"\n";
  output += torso->part_to_string()+""+head->part_to_string();
  output += locomotor->part_to_string();
  output += std::to_string(num_of_arms)+" "+arm->part_to_string();
  output += std::to_string(num_of_batteries)+" "+battery->part_to_string();
  return output;
}
// /////////////////////////////////////
//              V I E W
// /////////////////////////////////////
class View {
  public:
    View(vector<RobotPart*>& rps, vector<RobotModel*>& rms)
     : robotparts(rps), robotmodels(rms) { }
    string get_main_menu();
    string get_create_menu();
    string get_part_menu();
    string get_report_menu();

    string get_part_list();
    string get_part_list(string type);
    string get_model_list();
  private:
    vector<RobotPart*>& robotparts;
    vector<RobotModel*>& robotmodels;
};

string View::get_main_menu() {
  string menu =
  R"(
  Main Menu
  ---------
  1 Create
  2 Report
  0 Exit Program

  )";
  return menu;
}
string View::get_create_menu() {
  string menu =
  R"(
  Create
  ------
  1 Part
  2 Model
  3 Easter Egg
  0 Exit to Main Menu

  )";
  return menu;
}
string View::get_part_menu() {
  string menu =
  R"(
  Select Part
  -----------
  1 Arm
  2 Battery
  3 Head
  4 Locomotor
  5 Torso
  0 Exit to Main Menu
  )";
  return menu;
}
string View::get_report_menu() {
  string menu =
  R"(
  Report
  ------
  1 Part
  2 Model
  0 Exit to Main Menu

  )";
  return menu;
}
string View::get_part_list() {
  if(robotparts.size() == 0) return "No parts available\n";
  string output;
  for(int i = 0; i < robotparts.size(); i++) {
    output += robotparts[i]->part_to_string();
  }
  return output;
}
string View::get_part_list(string type) {
  if(robotparts.size() == 0) return "No parts available\n";
  string output;
  for(int i = 0; i < robotparts.size(); i++) {
    if(robotparts[i]->getType() == type)
      output += robotparts[i]->part_to_string();
  }
  return output;
}
string View::get_model_list() {
  if(robotmodels.size() == 0) return "No models available.\n";
  string output;
  for(int i = 0; i < robotmodels.size(); i++) {
    output += robotmodels[i]->to_string();
  }
  return output;
}
// /////////////////////////////////////
//         C O N T R O L L E R
// /////////////////////////////////////
class Controller {
  public:
    Controller(vector<RobotPart*>& rps, vector<RobotModel*>& rms, View& view)
     : robotparts(rps), robotmodels(rms), view(view)  { }
    void main_interface();
    void main_runner(int choice);

    void create_interface();
    void create_runner(int choice);
    void part_interface();
    void create_part(int choice);
    void create_model();

    void report_interface();
    void report_runner(int choice);
  private:
    double get_double(string prompt);
    double get_double(string prompt, double max_double);
    int get_int(string prompt);
    int get_int(string prompt, int max_int);
    int get_int(string prompt, int min_int, int max_int);
    string get_string(string prompt);
    int get_part(string type);

    vector<RobotPart*>& robotparts;
    vector<RobotModel*>& robotmodels;
    View& view;
};

double Controller::get_double(string prompt) {
  double result;
  while(true) {
    cout << prompt;
    cin >> result;
    cin.ignore();
    if(0 < result) break;
    cout << "Please enter a value greater than zero" << endl;
  }
  return result;
}
int Controller::get_int(string prompt) {
  int result;
  while(true) {
    cout << prompt;
    cin >> result;
    cin.ignore();
    if(0 < result) break;
    cout << "Please enter an integer greater than zero" << endl;
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
int Controller::get_part(string type) {
  string partName, prompt;
  bool partExists = false;
  int partIndex;

  prompt = "Select a part.\n"+type+" name? ";
  cout << "Accessing "+type+" information...\n\n";

  cout << view.get_part_list(type);
  partName = get_string(prompt);
  for(int i = 0; i < robotparts.size(); i++) {
    if(robotparts[i]->getName() == partName) {
      //Resolve potential name ambiguity here
      partExists = true;
      partIndex = i;
    }
  }

  if(!partExists) {
    cout << "Fatal Error - Invalid part name.";
    cout << "Please restart the robot model creation process.";
    return -1;
  }
  return partIndex;
}

void Controller::main_interface() {
  int choice = -1;
  string prompt = view.get_main_menu();
  while(choice != 0) {
    choice = get_int(prompt, 2);
    main_runner(choice);
  }
  cout << "Exitting program...\n";
}
void Controller::main_runner(int choice) {
  if(choice == 0) return;
  if(choice == 1) {
    cout << "Navigating to create menu...\n";
    create_interface();
  } else if (choice == 2) {
    cout << "Navigating to report menu...\n";
    this->report_interface();
  }
}

void Controller::create_interface() {
  int choice = -1;
  string prompt = view.get_create_menu();
  while (choice != 0) {
    choice = get_int(prompt, 3);
    create_runner(choice);
  }
  cout << "Returning to main menu...\n";
}
void Controller::create_runner(int choice) {
  if(choice == 0) return;

  if(choice == 1) {
    cout << "Navigating to part menu...\n";
    this->part_interface();
  } else if(choice == 2) {
    cout << "Initializing robot model...\n";
    this->create_model();
  } else if(choice == 3) {
    cout << "Filling databases for testing...\n";
    RobotPart* part;
    RobotModel* model;

    string image_filename = "image.png";
    part = new Arm{"Arm", "ACME Arm", 1000, 49.95, 15, "Standard Issue", image_filename, 8999};
    robotparts.push_back(part);
    part = new Battery{"Battery", "ACME Battery", 1000, 14.95, 0.5, "Standard Issue", image_filename, 12000, 56000};
    robotparts.push_back(part);
    part = new Head{"Head", "ACME Head", 1000, 149.95, 25, "Standard Issue", image_filename, 8999};
    robotparts.push_back(part);
    part = new Locomotor{"Locomotor", "ACME Locomotor", 1000, 249.95, 75.5, "Standard Issue", image_filename, 8999};
    robotparts.push_back(part);
    part = new Torso{"Torso", "ACME Torso", 1000, 99.95, 120, "Standard Issue", image_filename, 2, 2};
    robotparts.push_back(part);

    model = new RobotModel{"ACME Robo", 1000, robotparts[4], robotparts[2], robotparts[3], robotparts[0], robotparts[1], 2, 2};
    robotmodels.push_back(model);

  }
}
void Controller::part_interface() {
  int choice = -1;
  string prompt = view.get_part_menu();
  while(choice != 0) {
    choice = get_int(prompt, 5);
    create_part(choice);
  }
  cout << "Returning to main menu...\n";
}
void Controller::create_part(int choice) {
  if(choice == 0) return;
  cout << "Gathering default robot part information...\n";
  string name, description, type, image_filename;
  int model_number;
  double cost, weight;

  name = get_string("Part Name? ");
  model_number = get_int("Model Number? ");
  cost = get_double("Cost[$]? ");
  weight = get_double("Weight[lbs]? ");
  description = get_string("Description? ");
  image_filename = "default.png";

  RobotPart* part;

  if(choice == 1) {
    cout << "Gathering unique robot arm information...\n";
    double max_power;

    type = "Arm";
    max_power = get_double("Max Power[W]? ");
    part = new Arm{type, name, model_number, cost, weight, description, image_filename, max_power};
    robotparts.push_back(part);

  } else if(choice == 2) {
    cout << "Gathering unique battery information...\n";
    double power_available, max_energy;

    type = "Battery";
    power_available = get_double("Max Power[W]? ");
    max_energy = get_double("Max Energy[kWh]? ");
    part = new Battery{type, name, model_number, cost, weight, description, image_filename, power_available, max_energy};
    robotparts.push_back(part);

  } else if(choice == 3) {
    cout << "Gathering unique robot head information...\n";
    double power;

    type = "Head";
    power = get_double("Power? ");
    part = new Head{type, name, model_number, cost, weight, description, image_filename, power};
    robotparts.push_back(part);

  } else if(choice == 4) {
    cout << "Gathering unique locomotor information...\n";
    double max_power;

    type = "Locomotor";
    max_power = get_double("Max Power[W]? ");
    part = new Locomotor{type, name, model_number, cost, weight, description, image_filename, max_power};
    robotparts.push_back(part);

  } else if(choice == 5) {
    cout << "Gathering unique robot torso information...\n";
    int battery_compartments, max_arms;

    type = "Torso";
    battery_compartments = get_int("Battery Compartments? ", 1, 3);
    max_arms = get_int("Max Arms? ", 1, 2);
    part = new Torso{type, name, model_number, cost, weight, description, image_filename, battery_compartments, max_arms};
    robotparts.push_back(part);

  }
}
void Controller::create_model() {
  if(robotparts.size() < 5) {
    cout << "Fatal Error - Less than 5 parts exist in the shop\'s inventory.";
    return;
  }

  string name, type;
  int model_number, partIndex;
  int maxArms, numOfArms;
  int maxBatteries, numOfBatteries;
  RobotPart* torso;
  RobotPart* head;
  RobotPart* locomotor;
  RobotPart* arm;
  RobotPart* battery;

  RobotModel* model;

  name = get_string("Model Name? ");
  model_number = get_int("Model Number? ");

  type = "Torso";
  partIndex = get_part(type);
  if(partIndex >= 0)
    torso = robotparts[partIndex];
  else
    return;

  type = "Head";
  partIndex = get_part(type);
  if(partIndex >= 0)
    head = robotparts[partIndex];
  else
    return;

  type = "Locomotor";
  partIndex = get_part(type);
  if(partIndex >= 0)
    locomotor = robotparts[partIndex];
  else
    return;

  type = "Arm";
  partIndex = get_part(type);
  if(partIndex >= 0)
    arm = robotparts[partIndex];
  else
    return;
  maxArms = static_cast<Torso*>(torso)->getMaxArms();
  numOfArms = get_int("How many arms? ", 1, maxArms);

  type = "Battery";
  partIndex = get_part(type);
  if(partIndex >= 0)
    battery = robotparts[partIndex];
  else
    return;
  maxBatteries = static_cast<Torso*>(torso)->getBatteryCompartments();
  numOfBatteries = get_int("How many batteries? ", 1, maxBatteries);

  model = new RobotModel{name, model_number, torso, head, locomotor, arm, battery, numOfBatteries, numOfArms};
  robotmodels.push_back(model);

}

void Controller::report_interface() {
  int choice = -1;
  string prompt = view.get_report_menu();
  while(choice != 0) {
    choice = get_int(prompt, 2);
    report_runner(choice);
  }
  cout << "Returning to main menu...\n";
}
void Controller::report_runner(int choice) {
  if(choice == 0) return;
  if(choice == 1) {
    cout << "Retrieving part data...\n";
    cout << view.get_part_list();
  } else if(choice == 2) {
    cout << "Retrieving model data...\n";
    cout << view.get_model_list();
  }
}

int main() {
  vector<RobotPart*> rps;
  vector<RobotModel*> rms;
  View view{rps, rms};
  Controller controller{rps, rms, view};
  controller.main_interface();
}
