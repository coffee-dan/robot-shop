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
double get_double(string prompt) {
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
int get_int(string prompt) {
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
int get_int(string prompt, int max_int) {
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
int get_int(string prompt, int min_int, int max_int) {
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
string get_string(string prompt) {
  string result;
  cout << prompt;
  getline(cin, result);
  return result;
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
  string output = name+", Model #"+std::to_string(model_number)+"\n"+image_filename+", Cost : $"+dtos(cost, 2);
  output += "\nDescription : "+description+"\nWeight : "+dtos(weight, 2)+" [lbs]\n";
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
  return type+" : "+output+"Max Power : "+dtos(max_power, 2)+" [W]\n\n";
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
  return type+" : "+output +"Power Available : "+dtos(power_available, 2)+" [W], Max Energy : "+dtos(max_energy, 2)+" [kWh]\n\n";
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
  return type+" : "+output +"Power : "+dtos(power, 2)+" [W]\n\n";
}
// /////////////////////////////////////
//         L O C O M O T O R
// /////////////////////////////////////
class Locomotor : public RobotPart {
  public:
    Locomotor(string t, string n, int m, double c, double w, string d, string i, double m_p, double m_s)
      : RobotPart(t, n, m, c, w, d, i), max_power(m_p), max_speed(m_s) { }
    double getMaxPower() { return max_power; }
    double getMaxSpeed() { return max_speed; }
    string part_to_string();
  private:
    double max_power;
    double max_speed;
};
string Locomotor::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"Max Power : "+dtos(max_power, 2)+" [W], Max Speed : "+dtos(max_speed, 2)+" [mph]\n\n";
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
  return type+" : "+output +"Battery Compartments : "+std::to_string(battery_compartments)+", Max Arms : "+std::to_string(max_arms)+"\n\n";
}
// /////////////////////////////////////
//         R O B O T   M O D E L
// /////////////////////////////////////
class RobotModel {
  public:
    RobotModel(string n, int m, RobotPart* t, RobotPart* h, RobotPart* l, vector<RobotPart*> a, vector<RobotPart*> b, int numOfB, int numOfA)
      :  name(n), model_number(m), torso(t), head(h), locomotor(l), arms(a), batteries(b), num_of_batteries(numOfB), num_of_arms(numOfA) { }
    RobotPart* getTorso() { return torso; }
    RobotPart* getHead() { return head; }
    RobotPart* getLocomotor() { return locomotor; }
    RobotPart* getArm(int index) { return arms[index]; }
    RobotPart* getBattery(int index) { return batteries[index]; }
    double total_weight();
    double cost_of_parts();
    double max_speed();
    double battery_life();
    string to_string();
  private:
    string name;
    int model_number;
    RobotPart* torso;
    RobotPart* head;
    RobotPart* locomotor;
    vector<RobotPart*> arms;
    vector<RobotPart*> batteries;
    int num_of_arms;
    int num_of_batteries;
};

double RobotModel::total_weight() {
  double weight = torso->getWeight() + head->getWeight();
  weight += locomotor->getWeight();
  for(int i = 0; i < num_of_arms; i++) {
    weight += arms[i]->getWeight();
  }
  for(int i = 0; i < num_of_batteries; i++) {
    weight += batteries[i]->getWeight();
  }
  return weight;
}
double RobotModel::cost_of_parts() {
  double cost = torso->getCost() + head->getCost();
  cost += locomotor->getCost();
  for(int i = 0; i < num_of_arms; i++) {
    cost += arms[i]->getCost();
  }
  for(int i = 0; i < num_of_batteries; i++) {
    cost += batteries[i]->getCost();
  }
  return cost;
}
double RobotModel::max_speed() {
  double rated_weight = 5*(locomotor->getWeight());
  double rated_speed = static_cast<Locomotor*>(locomotor)->getMaxSpeed();
  double model_weight = total_weight();
  if(rated_weight > model_weight)
    return rated_speed*(rated_weight/model_weight);
  else
    return rated_speed;
}
double RobotModel::battery_life() {
  double power_consumption = static_cast<Head*>(head)->getPower();
  double arm_power = 0;
  for(int i = 0; i < num_of_arms; i++) {
    arm_power += static_cast<Arm*>(arms[i])->getMaxPower();
  }
  power_consumption += 0.40 * arm_power;
  power_consumption += 0.15 * static_cast<Locomotor*>(locomotor)->getMaxPower();

  double model_energy;
  for(int i = 0; i < num_of_batteries; i++) {
    model_energy += static_cast<Battery*>(batteries[i])->getMaxEnergy();
  }
  model_energy *= 1000; //From kWh to Wh

  return model_energy/power_consumption;
}
string RobotModel::to_string() {
  string output = "Robot Model : "+name+", Model #"+std::to_string(model_number);
  output += " - $"+dtos(cost_of_parts(), 2)+"\nMax Speed : "+dtos(max_speed(), 2)+" [mph], Max Battery Life : ";
  output += dtos(battery_life(), 2)+"\n\n";

  output += torso->part_to_string()+""+head->part_to_string()+""+locomotor->part_to_string();

  for(int i = 0; i < num_of_arms; i++) {
    output += "\tArm "+std::to_string(i+1)+" of "+std::to_string(num_of_arms)+"\n"+arms[i]->part_to_string();
  }
  for(int i = 0; i < num_of_batteries; i++) {
    output += "\tBattery "+std::to_string(i+1)+" of "+std::to_string(num_of_arms)+"\n"+batteries[i]->part_to_string();
  }

  return output;
}

// /////////////////////////////////////
//    B E L O V E D   C U S T O M E R
// /////////////////////////////////////
class Customer {
  public:
    Customer(string _name, int _customer_num, string _phone_num, string _email)
    : name(_name), customer_number(_customer_num), phone_number(_phone_num), email_address(_email) { }
    string getName() { return name; }
    int getCustomerNumber() { return customer_number; }
    string getPhoneNumber() { return phone_number; }
    string getEmailAddress() { return email_address; }
    string to_string();
  private:
    string name;
    int customer_number;
    string phone_number;
    string email_address;
};
string Customer::to_string() {
  string output = name+" - "+std::to_string(customer_number)+'\n';
  output += phone_number+", "+email_address;
  return output;
}
// /////////////////////////////////////
//     S A L E S   A S S O C I A T E
// /////////////////////////////////////
class SalesAssociate {

};
// /////////////////////////////////////
//              S H O P
// /////////////////////////////////////
class Shop {
  public:
    Shop(vector<RobotPart*>& rps, vector<RobotModel*> rms, vector<Customer*> cs)
     : robotparts(rps), robotmodels(rms), customers(cs) { }

    void create_new_robot_part(int choice);
    RobotPart* get_part(int index) { return robotparts[index]; }
    string part_to_string(int index);
    string part_list_to_string();
    string part_list_to_string(string type);
    int num_of_parts() { return robotparts.size(); }

    void create_new_robot_model();
    RobotModel* get_model(int index) { return robotmodels[index]; }
    string model_to_string(int index);
    string model_list_to_string();
    int num_of_models() { return robotmodels.size(); }

    void create_new_customer();
    Customer* get_customer(int index) { return customers[index]; }
    string customer_to_string(int index);
    string customer_list_to_string();
    int num_of_customers() { return customers.size(); }

    void easter_egg();
  private:
    int get_robot_part(string type);

    vector<RobotPart*>& robotparts;
    vector<RobotModel*>& robotmodels;
    vector<Customer*>& customers;
};

void Shop::create_new_robot_part(int choice) {
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
    double max_power, max_speed;

    type = "Locomotor";
    max_power = get_double("Max Power[W]? ");
    max_speed = get_double("Max Speed[mph]? ");
    part = new Locomotor{type, name, model_number, cost, weight, description, image_filename, max_power, max_speed};
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
int Shop::get_robot_part(string type) {
  string partName, prompt;
  bool partExists = false;
  bool partsAvailable = false;
  int partIndex;

  prompt = "Select a part.\n"+type+" name? ";

  cout << "Accessing "+type+" information...\n\n";

  while(true) {
    cout << part_list_to_string(type);

    partName = get_string(prompt);
    for(int i = 0; i < robotparts.size(); i++) {
      if(robotparts[i]->getType() == type) partsAvailable = true;
      if((robotparts[i]->getType() == type) & (robotparts[i]->getName() == partName)) {
        partExists = true;
        partIndex = i;
      }
    }

    if(!partsAvailable) break;
    if(partExists & partsAvailable) break;
    cout << "Error - Please re-enter part name.\n";
  }

  if(!partsAvailable) {
    cout << "Fatal Error - No "+type+" parts available.\n";
    return -1;
  }
  return partIndex;
}
string Shop::part_to_string(int index) {
  return robotparts[index]->part_to_string();
}
string Shop::part_list_to_string() {
  if(robotparts.size() == 0) return "No parts available\n";
  string output;
  for(int i = 0; i < robotparts.size(); i++) {
    output += part_to_string(i);
  }
  return output;
}
string Shop::part_list_to_string(string type) {
  if(robotparts.size() == 0) return "No parts available\n";
  string output;
  for(int i = 0; i < robotparts.size(); i++) {
    if(robotparts[i]->getType() == type)
      output += part_to_string(i);
  }
  return output;
}

void Shop::create_new_robot_model() {
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
  vector<RobotPart*> arms;
  vector<RobotPart*> batteries;

  RobotModel* model;

  name = get_string("Model Name? ");
  model_number = get_int("Model Number? ");

  type = "Torso";
  partIndex = get_robot_part(type);
  if(partIndex >= 0)
    torso = robotparts[partIndex];
  else
    return;

  type = "Head";
  partIndex = get_robot_part(type);
  if(partIndex >= 0)
    head = robotparts[partIndex];
  else
    return;

  type = "Locomotor";
  partIndex = get_robot_part(type);
  if(partIndex >= 0)
    locomotor = robotparts[partIndex];
  else
    return;

  maxArms = static_cast<Torso*>(torso)->getMaxArms();
  numOfArms = get_int("How many arms? ", 1, maxArms);

  for(int i = 0; i < numOfArms; i++) {
    type = "Arm";
    partIndex = get_robot_part(type);
    if(partIndex >= 0)
      arms.push_back(robotparts[partIndex]);
    else
      return;
  }

  maxBatteries = static_cast<Torso*>(torso)->getBatteryCompartments();
  numOfBatteries = get_int("How many batteries? ", 1, maxBatteries);

  for(int i = 0; i < numOfBatteries; i++) {
    type = "Battery";
    partIndex = get_robot_part(type);
    if(partIndex >= 0)
      batteries.push_back(robotparts[partIndex]);
    else
      return;
  }

  model = new RobotModel{name, model_number, torso, head, locomotor, arms, batteries, numOfBatteries, numOfArms};
  robotmodels.push_back(model);

}
string Shop::model_to_string(int index) {
  return robotmodels[index]->to_string();
}
string Shop::model_list_to_string() {
  if(robotmodels.size() == 0) return "No models available.\n";
  string output;
  for(int i = 0; i < robotmodels.size(); i++) {
    output += model_to_string(i);
  }
  return output;
}

void Shop::create_new_customer() {
  cout << "Gathering customer information...\n";
  string name, phone_number, email_address;
  int customer_number;

  name = get_string("Name[First and Last]? ");
  phone_number = get_string("Phone Number? ");
  email_address = get_string("Email Address? ");
  customer_number = get_int("Customer ID#? ");

  Customer* customer = new Customer{name, customer_number, phone_number, email_address};
  customers.push_back(customer);

}
string Shop::customer_to_string(int index) {
  return customers[index]->to_string();
}
string Shop::customer_list_to_string() {
  if(customers.size() == 0) return "No customer accounts exist.\n";
  string output;
  for(int i = 0; i < customers.size(); i++) {
    output += customer_to_string(i);
  }
  return output;
}

void Shop::easter_egg() {
  cout << "Filling databases for testing...\n";
  string image_filename = "image.png";
  RobotPart* arm;
  RobotPart* battery;
  RobotPart* head;
  RobotPart* locomotor;
  RobotPart* torso;

  vector<RobotPart*> arms;
  vector<RobotPart*> batteries;

  RobotModel* model;

  arm = new Arm{"Arm", "ACME Arm", 1000, 49.95, 15, "Standard Issue", image_filename, 8999};
  robotparts.push_back(arm);
  arms.push_back(arm);
  arms.push_back(arm);
  battery = new Battery{"Battery", "ACME Battery", 1000, 14.95, 0.5, "Standard Issue", image_filename, 12000, 56000};
  robotparts.push_back(battery);
  batteries.push_back(battery);
  batteries.push_back(battery);

  head = new Head{"Head", "ACME Head", 1000, 149.95, 25, "Standard Issue", image_filename, 8999};
  robotparts.push_back(head);
  locomotor = new Locomotor{"Locomotor", "ACME Locomotor", 1000, 249.95, 75.5, "Standard Issue", image_filename, 8999, 500.5};
  robotparts.push_back(locomotor);
  torso = new Torso{"Torso", "ACME Torso", 1000, 99.95, 120, "Standard Issue", image_filename, 2, 2};
  robotparts.push_back(torso);

  model = new RobotModel{"ACME Robo", 1000, torso, head, locomotor, arms, batteries, 2, 2};
  robotmodels.push_back(model);
}
// /////////////////////////////////////
//              V I E W
// /////////////////////////////////////
class View {
  public:
    View(Shop& shop)
     : shop(shop) { }
    string get_main_menu();
    string get_create_menu();
    string get_part_menu();
    string get_report_menu();

    string get_part_list();
    string get_part_list(string type);
    string get_model_list();
    string get_customer_list();
  private:
    Shop& shop;
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
  1 Customer
  2 Part
  3 Model
  4 Easter Egg
  0 Return to Main Menu

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
  0 Return to Main Menu

  )";
  return menu;
}
string View::get_report_menu() {
  string menu =
  R"(
  Report
  ------
  1 Custmer
  2 Part
  3 Model
  0 Return to Main Menu

  )";
  return menu;
}

string View::get_part_list() {
  return shop.part_list_to_string();
}
string View::get_part_list(string type) {
  return shop.part_list_to_string(type);
}
string View::get_model_list() {
  return shop.model_list_to_string();
}
string View::get_customer_list() {
  return shop.customer_list_to_string();
}
// /////////////////////////////////////
//         C O N T R O L L E R
// /////////////////////////////////////
class Controller {
  public:
    Controller(Shop& shop, View& view)
     : shop(shop), view(view)  { }
    void main_interface();
    void main_runner(int choice);

    void create_interface();
    void create_runner(int choice);
    void part_interface();
    void part_runner(int choice);

    void report_interface();
    void report_runner(int choice);
  private:
    Shop& shop;
    View& view;
};

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
    report_interface();
  }
}

void Controller::create_interface() {
  int choice = -1;
  string prompt = view.get_create_menu();
  while (choice != 0) {
    choice = get_int(prompt, 4);
    create_runner(choice);
  }
  cout << "Returning to main menu...\n";
}
void Controller::create_runner(int choice) {
  if(choice == 0) return;

  if(choice == 1) {
    cout << "Initializing customer account...\n";
    shop.create_new_customer();
  } else if(choice == 2) {
    cout << "Navigating to part menu...\n";
    part_interface();
  } else if(choice == 3) {
    cout << "Initializing robot model...\n";
    shop.create_new_robot_model();
  } else if(choice == 4) {
    shop.easter_egg();
  }
}
void Controller::part_interface() {
  int choice = -1;
  string prompt = view.get_part_menu();
  while(choice != 0) {
    choice = get_int(prompt, 5);
    part_runner(choice);
  }
  cout << "Returning to main menu...\n";
}
void Controller::part_runner(int choice) {
  if(choice == 0) return;
  shop.create_new_robot_part(choice);
}

void Controller::report_interface() {
  int choice = -1;
  string prompt = view.get_report_menu();
  while(choice != 0) {
    choice = get_int(prompt, 3);
    report_runner(choice);
  }
  cout << "Returning to main menu...\n";
}
void Controller::report_runner(int choice) {
  if(choice == 0) return;
  if(choice == 1) {
    cout << "Retrieving customer data...\n";
    cout << view.get_customer_list();
  } else if(choice == 2) {
    cout << "Retrieving part data...\n";
    cout << view.get_part_list();
  } else if(choice == 3) {
    cout << "Retrieving model data...\n";
    cout << view.get_model_list();
  }
}

int main() {
  vector<RobotPart*> rps;
  vector<RobotModel*> rms;
  vector<Customer*> cs;
  Shop shop{rps, rms, cs};
  View view{shop};
  Controller controller{shop, view};
  controller.main_interface();
}
