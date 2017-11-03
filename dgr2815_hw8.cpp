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
    RobotPart(string t, string n, int m, double c, string d, string i)
      : type(t), name(n), model_number(m), cost(c), description(d), image_filename(i) { }
    double get_cost();
    string dtos(double num, int precision);
    string to_string();
    virtual string part_to_string()=0;
  protected:
    string type;
    string name;
    int model_number;
    double cost;
    string description;
    string image_filename;
};

double RobotPart::get_cost() {
  return cost;
}
string RobotPart::dtos(double num, int precision) {
  stringstream ss;
  ss << fixed << setprecision(precision) << num;
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
    Arm(string t, string n, int m, double c, string d, string i, double m_p)
      : RobotPart(t, n, m, c, d, i), max_power(m_p) { }
    string part_to_string();
  private:
    double max_power;
};

string Arm::part_to_string() {
  string output = RobotPart::to_string();
  return "--"+type+"--\n"+output + "\nMax Power : "+dtos(max_power, 2)+"\n";
}
// /////////////////////////////////////
//          B A T T E R Y
// /////////////////////////////////////
class Battery : public RobotPart {
  public:
    Battery(string t, string n, int m, double c, string d, string i, double p, double m_e)
      : RobotPart(t, n, m, c, d, i), power_available(p), max_energy(m_e) { }
    string part_to_string();
  private:
    double power_available;
    double max_energy;
};

string Battery::part_to_string() {
  string output = RobotPart::to_string();
  return "--"+type+"--\n"+output +"\nPower Available : "+std::to_string(power_available)+"\nMax Energy : "+dtos(max_energy, 2)+"\n";
}
// /////////////////////////////////////
//             H E A D
// /////////////////////////////////////
class Head : public RobotPart {
  public:
    Head(string t, string n, int m, double c, string d, string i, double p)
      :  RobotPart(t, n, m, c, d, i), power(p) { }
    string part_to_string();
  private:
    double power;
};

string Head::part_to_string() {
  string output = RobotPart::to_string();
  return "--"+type+"--\n"+output +"\nPower : "+dtos(power, 2)+"\n";
}
// /////////////////////////////////////
//         L O C O M O T O R
// /////////////////////////////////////
class Locomotor : public RobotPart {
  public:
    Locomotor(string t, string n, int m, double c, string d, string i, double m_p)
      : RobotPart(t, n, m, c, d, i), max_power(m_p) { }
    string part_to_string();
  private:
    double max_power;
};

string Locomotor::part_to_string() {
  string output = RobotPart::to_string();
  return "--"+type+"--\n"+output +"\nMax Power : "+dtos(max_power, 2)+"\n";
}
// /////////////////////////////////////
//            T O R S O
// /////////////////////////////////////
class Torso : public RobotPart {
  public:
    Torso(string t, string n, int m, double c, string d, string i, int b, int m_a)
      :  RobotPart(t, n, m, c, d, i), battery_compartments(b), max_arms(m_a) { }
    string part_to_string();
  private:
    int battery_compartments;
    int max_arms;
};

string Torso::part_to_string() {
  string output = RobotPart::to_string();
  return "--"+type+"--\n"+output +"\nBattery Compartments : "+std::to_string(battery_compartments)+"\nMax Arms : "+std::to_string(max_arms)+"\n";
}
// /////////////////////////////////////
//         R O B O T   M O D E L
// /////////////////////////////////////
class RobotModel {
  public:
    RobotModel(string n, int m, RobotPart* t, RobotPart* h, RobotPart* l, RobotPart* a, RobotPart* b, int numOfB, int numOfA)
      :  name(n), model_number(m), torso(t), head(h), locomotor(l), arm(a), battery(b), num_of_batteries(numOfB), num_of_arms(numOfA) { }
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
    int num_of_batteries;
    int num_of_arms;
};
double RobotModel::cost() {
  double cost = torso->get_cost()+head->get_cost()+locomotor->get_cost()+(num_of_arms*arm->get_cost())+(num_of_batteries*battery->get_cost());
  return cost;
}
// /////////////////////////////////////
//              V I E W
// /////////////////////////////////////
class View {
  public:
    View(vector<RobotPart*>& rps) : robotparts(rps) { }
    string get_main_menu();
    string get_part_menu();
    string get_part_list();
  private:
    vector<RobotPart*>& robotparts;
};

string View::get_main_menu() {
  string menu = R"(
=======================
Welcome to robot shop !
=======================

  Robot
  -----
  Create part :: 1
  List parts  :: 2

  Utility
  -------
  Exit        :: 0

)";
  return menu;
}
string View::get_part_menu() {
  string menu = R"(
##################
Which robot part??
##################
  Arm       :: 1
  Battery   :: 2
  Head      :: 3
  Locomotor :: 4
  Torso     :: 5

  Exit      :: 0

)";
  return menu;
}
string View::get_part_list() {
  string output;
  for(int i = 0; i < robotparts.size(); i++) {
    output += robotparts[i]->part_to_string();
  }
  return output;
}
// /////////////////////////////////////
//         C O N T R O L L E R
// /////////////////////////////////////
class Controller {
  public:
    Controller(vector<RobotPart*>& rps, View& view) : robotparts(rps), view(view)  { }
    void cli();
    void execute_cmd(int cmd);
    void part_interface();
    void create_part(int choice);
  private:
    double get_double(string prompt);
    double get_double(string prompt, double max_double);
    int get_int(string prompt);
    int get_int(string prompt, int max_int);
    int get_int(string prompt, int min_int, int max_int);
    string get_string(string prompt);
    vector<RobotPart*>& robotparts;
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

void Controller::cli() {
  int cmd = -1;
  string prompt = view.get_main_menu();
  while(cmd != 0) {
    cmd = get_int(prompt, 2);
    execute_cmd(cmd);
  }
  cout << "Exitting program...\n";
}
void Controller::execute_cmd(int cmd) {
  if(cmd == 0) return;
  if(cmd == 1) {
    this->part_interface();
  } else if (cmd == 2) {
    cout << view.get_part_list();
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
  double cost;

  name = get_string("Name? ");
  model_number = get_int("Model Number? ");
  cost = get_double("Cost[$]? ");
  description = get_string("Description? ");
  image_filename = "default.png";

  RobotPart* part;

  if(choice == 1) {
    cout << "Gathering unique robot arm information...\n";
    double max_power;

    type = "Arm";
    max_power = get_double("Max Power[W]? ");
    part = new Arm{type, name, model_number, cost, description, image_filename, max_power};
    robotparts.push_back(part);

  } else if(choice == 2) {
    cout << "Gathering unique battery information...\n";
    double power_available, max_energy;

    type = "Battery";
    power_available = get_double("Max Power[W]? ");
    max_energy = get_double("Max Energy[kWh]? ");
    part = new Battery{type, name, model_number, cost, description, image_filename, power_available, max_energy};
    robotparts.push_back(part);

  } else if(choice == 3) {
    cout << "Gathering unique robot head information...\n";
    double power;

    type = "Head";
    power = get_double("Power? ");
    part = new Head{type, name, model_number, cost, description, image_filename, power};
    robotparts.push_back(part);

  } else if(choice == 4) {
    cout << "Gathering unique locomotor information...\n";
    double max_power;

    type = "Locomotor";
    max_power = get_double("Max Power[W]? ");
    part = new Locomotor{type, name, model_number, cost, description, image_filename, max_power};
    robotparts.push_back(part);

  } else if(choice == 5) {
    cout << "Gathering unique robot torso information...\n";
    int battery_compartments, max_arms;

    type = "Torso";
    battery_compartments = get_int("Battery Compartments? ", 1, 3);
    max_arms = get_int("Max Arms? ", 1, 2);
    part = new Torso{type, name, model_number, cost, description, image_filename, battery_compartments, max_arms};
    robotparts.push_back(part);

  }
}

int main() {
  vector<RobotPart*> rps;
  View view{rps};
  Controller controller{rps, view};
  controller.cli();
}
