#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/fl_ask.H>

using namespace std;

string dtos(double num, int precision) {
  stringstream ss;
  ss << fixed << setprecision(precision) << num;
  return ss.str();
}
double get_double(string title, string prompt) {
  string error = "Please enter a value greater than zero";
  double result;
  while(true) {
		fl_message_title(title.c_str());
    fl_message_icon()->label("?");
		result = stoi(fl_input("%s", 0, prompt.c_str()));
    if (0 < result) break;
		fl_message_title("Invalid input");
    fl_message_icon()->label("!");
    fl_message("%s", error.c_str());
  }
  return result;
}
int get_int(string title, string prompt) {
  string error = "Please enter an integer greater than zero";
  int result;
  while(true) {
		fl_message_title(title.c_str());
    fl_message_icon()->label("?");
		result = stoi(fl_input("%s", 0, prompt.c_str()));
    if (0 <= result) break;
		fl_message_title("Invalid input");
    fl_message_icon()->label("!");
    fl_message("%s", error.c_str());
  }
  return result;
}
int get_int(string title, string prompt, int max_int) {
  string error = "Please enter an integer between 0 and "+max_int;
  int result;
  while(true) {
		fl_message_title(title.c_str());
    fl_message_icon()->label("?");
		result = stoi(fl_input("%s", 0, prompt.c_str()));
    if (0 <= result && result <= max_int) break;
		fl_message_title("Invalid input");
    fl_message_icon()->label("!");
    fl_message("%s", error.c_str());
  }
  return result;
}
int get_int(string title, string prompt, int min_int, int max_int) {
  string error = "Please enter an integer between "+std::to_string(min_int)+" and "+std::to_string(max_int);
  int result;
  while(true) {
		fl_message_title(title.c_str());
    fl_message_icon()->label("?");
		result = stoi(fl_input("%s", 0, prompt.c_str()));
    if (min_int <= result && result <= max_int) break;
		fl_message_title("Invalid input");
    fl_message_icon()->label("!");
    fl_message("%s", error.c_str());
  }
  return result;
}
string get_string(string prompt) {
  string title = "Robot Bois";
  fl_message_title(title.c_str());
  fl_message_icon()->label("?");
  string result{fl_input("%s", 0, prompt.c_str())};
  return result;
}
string get_string(string title, string prompt) {
 fl_message_title(title.c_str());
 fl_message_icon()->label("?");
 string result{fl_input("%s", 0, prompt.c_str())};
 return result;
}
void display_message(string title, string icon, string message) {
  fl_message_title(title.c_str());
	fl_message_icon()->label(icon.c_str());
	fl_message("%s", message.c_str());
}
void display_message(string title, string message) {
  fl_message_title(title.c_str());
	fl_message_icon()->label("!");
	fl_message("%s", message.c_str());
}
// /////////////////////////////////////
//        R O B O T   P A R T
// /////////////////////////////////////
class RobotPart {
  public:
    RobotPart(string t, string n, int m, double c, double w, string d, string i)
     : type(t), name(n), model_number(m), cost(c), weight(w), description(d), image_filename(i) { }
    RobotPart()
     : type(), name(), model_number(), cost(), description(), image_filename() { }
    string getType() { return type; }
    string getName() { return name; }
    string getModelNumber() { return std::to_string(model_number); }
    double getCost() { return cost; }
    double getWeight() { return weight; }

    string to_string();
    virtual string part_to_string()=0;
    string export_data() const;
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
string RobotPart::export_data() const{
  return name+'|'+std::to_string(model_number)+'|'+dtos(cost,2)+'|'+dtos(weight,2)+'|'+description+'|'+image_filename;
}
// /////////////////////////////////////
//              A R M
// /////////////////////////////////////
class Arm : public RobotPart {
  public:
    Arm(string t, string n, int m, double c, double w, string d, string i, double m_p)
     : RobotPart(t, n, m, c, w, d, i), max_power(m_p) { }
    Arm()
     : RobotPart(), max_power() { }
    double getMaxPower() { return max_power; }
    string part_to_string();
    friend ofstream& operator<<(ofstream& ofs, const Arm arm);
    friend istringstream& operator>>(istringstream& is, Arm& arm);
  private:
    double max_power;
};
string Arm::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output+"Max Power : "+dtos(max_power, 2)+" [W]\n\n";
}
ofstream& operator<<(ofstream& ofs, const Arm arm) {
  ofs << arm.export_data()+'|'+dtos(arm.max_power,2);
  return ofs;
}
istringstream& operator>>(istringstream& is, Arm& arm) {
  string _type = "Arm";
  string _name, _model_number, _cost, _weight, _description, _image_filename, _max_power;
  int delcount = 1;
  for(char c; is.get(c);) {
    if(c == '|') {
      delcount++;
    } else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _name += c;
    } else if(delcount == 2 && isdigit(c)) {
      _model_number += c;
    } else if(delcount == 3 && (isdigit(c) || c == '.')) {
      _cost += c;
    } else if(delcount == 4 && (isdigit(c) || c == '.')) {
      _weight += c;
    } else if(delcount == 5 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _description += c;
    } else if(delcount == 6 && (isalnum(c) || c == '.')) {
      _image_filename += c;
    } else if(delcount == 7 && (isdigit(c) || c == '.')) {
      _max_power += c;
    }

    if(delcount > 8) break;
  }
  arm.type = _type;
  arm.name = _name;
  arm.model_number = stoi(_model_number);
  arm.cost = stod(_cost);
  arm.weight = stod(_weight);
  arm.description = _description;
  arm.max_power = stod(_max_power);
  arm.image_filename = _image_filename;
}
// /////////////////////////////////////
//          B A T T E R Y
// /////////////////////////////////////
class Battery : public RobotPart {
  public:
    Battery(string t, string n, int m, double c, double w, string d, string i, double p, double m_e)
     : RobotPart(t, n, m, c, w, d, i), power_available(p), max_energy(m_e) { }
    Battery()
     : RobotPart(), power_available(), max_energy() { }
    double getPowerAvailable() { return power_available; }
    double getMaxEnergy() { return max_energy; }
    string part_to_string();
    friend ofstream& operator<<(ofstream& ofs, const Battery battery);
    friend istringstream& operator>>(istringstream& is, Battery& battery);
  private:
    double power_available;
    double max_energy;
};
string Battery::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"Power Available : "+dtos(power_available, 2)+" [W], Max Energy : "+dtos(max_energy, 2)+" [kWh]\n\n";
}
ofstream& operator<<(ofstream& ofs, const Battery battery) {
  ofs << battery.export_data()+'|'+dtos(battery.power_available,2)+'|'+dtos(battery.max_energy,2);
  return ofs;
}
istringstream& operator>>(istringstream& is, Battery& battery) {
  string _type = "Battery";
  string _name, _model_number, _cost, _weight, _description, _image_filename, _power_available, _max_energy;
  int delcount = 1;
  for(char c; is.get(c);) {
    if(c == '|') {
      delcount++;
    } else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _name += c;
    } else if(delcount == 2 && isdigit(c)) {
      _model_number += c;
    } else if(delcount == 3 && (isdigit(c) || c == '.')) {
      _cost += c;
    } else if(delcount == 4 && (isdigit(c) || c == '.')) {
      _weight += c;
    } else if(delcount == 5 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _description += c;
    } else if(delcount == 6 && (isalnum(c) || c == '.')) {
      _image_filename += c;
    } else if(delcount == 7 && (isdigit(c) || c == '.')) {
      _power_available += c;
    } else if(delcount == 8 && (isdigit(c) || c == '.')) {
      _max_energy += c;
    }

    if(delcount > 8) break;
  }
  battery.type = _type;
  battery.name = _name;
  battery.model_number = stoi(_model_number);
  battery.cost = stod(_cost);
  battery.weight = stod(_weight);
  battery.description = _description;
  battery.image_filename = _image_filename;
  battery.power_available = stod(_power_available);
  battery.max_energy = stod(_max_energy);
}
// /////////////////////////////////////
//             H E A D
// /////////////////////////////////////
class Head : public RobotPart {
  public:
    Head(string t, string n, int m, double c, double w, string d, string i, double p)
     : RobotPart(t, n, m, c, w, d, i), power(p) { }
    Head()
     : RobotPart(), power() { }
    double getPower() { return power; }
    string part_to_string();
    friend ofstream& operator<<(ofstream& ofs, const Head head);
    friend istringstream& operator>>(istringstream& is, Head& head);
  private:
    double power;
};
string Head::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"Power : "+dtos(power, 2)+" [W]\n\n";
}
ofstream& operator<<(ofstream& ofs, const Head head) {
  ofs << head.export_data()+'|'+dtos(head.power,2);
  return ofs;
}
istringstream& operator>>(istringstream& is, Head& head) {
  string _type = "Head";
  string _name, _model_number, _cost, _weight, _description, _image_filename, _power;
  int delcount = 1;
  for(char c; is.get(c);) {
    if(c == '|') {
      delcount++;
    } else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _name += c;
    } else if(delcount == 2 && isdigit(c)) {
      _model_number += c;
    } else if(delcount == 3 && (isdigit(c) || c == '.')) {
      _cost += c;
    } else if(delcount == 4 && (isdigit(c) || c == '.')) {
      _weight += c;
    } else if(delcount == 5 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _description += c;
    } else if(delcount == 6 && (isalnum(c) || c == '.')) {
      _image_filename += c;
    } else if(delcount == 7 && (isdigit(c) || c == '.')) {
      _power += c;
    }

    if(delcount > 7) break;
  }
  head.type = _type;
  head.name = _name;
  head.model_number = stoi(_model_number);
  head.cost = stod(_cost);
  head.weight = stod(_weight);
  head.description = _description;
  head.image_filename = _image_filename;
  head.power = stod(_power);
}
// /////////////////////////////////////
//         L O C O M O T O R
// /////////////////////////////////////
class Locomotor : public RobotPart {
  public:
    Locomotor(string t, string n, int m, double c, double w, string d, string i, double m_p, double m_s)
     : RobotPart(t, n, m, c, w, d, i), max_power(m_p), max_speed(m_s) { }
    Locomotor()
     : RobotPart(), max_power(), max_speed() { }
    double getMaxPower() { return max_power; }
    double getMaxSpeed() { return max_speed; }
    string part_to_string();
    friend ofstream& operator<<(ofstream& ofs, const Locomotor locomotor);
    friend istringstream& operator>>(istringstream& is, Locomotor& locomotor);
  private:
    double max_power;
    double max_speed;
};
string Locomotor::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"Max Power : "+dtos(max_power, 2)+" [W], Max Speed : "+dtos(max_speed, 2)+" [mph]\n\n";
}
ofstream& operator<<(ofstream& ofs, const Locomotor locomotor) {
  ofs << locomotor.export_data()+'|'+dtos(locomotor.max_power,2)+'|'+dtos(locomotor.max_speed,2);
  return ofs;
}
istringstream& operator>>(istringstream& is, Locomotor& locomotor) {
  string _type = "Locomotor";
  string _name, _model_number, _cost, _weight, _description, _image_filename, _max_power, _max_speed;
  int delcount = 1;
  for(char c; is.get(c);) {
    if(c == '|') {
      delcount++;
    } else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _name += c;
    } else if(delcount == 2 && isdigit(c)) {
      _model_number += c;
    } else if(delcount == 3 && (isdigit(c) || c == '.')) {
      _cost += c;
    } else if(delcount == 4 && (isdigit(c) || c == '.')) {
      _weight += c;
    } else if(delcount == 5 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _description += c;
    } else if(delcount == 6 && (isalnum(c) || c == '.')) {
      _image_filename += c;
    } else if(delcount == 7 && (isdigit(c) || c == '.')) {
      _max_power += c;
    } else if(delcount == 8 && (isdigit(c) || c == '.')) {
      _max_speed += c;
    }

    if(delcount > 8) break;
  }
  locomotor.type = _type;
  locomotor.name = _name;
  locomotor.model_number = stoi(_model_number);
  locomotor.cost = stod(_cost);
  locomotor.weight = stod(_weight);
  locomotor.description = _description;
  locomotor.image_filename = _image_filename;
  locomotor.max_power = stod(_max_power);
  locomotor.max_speed = stod(_max_speed);
}
// /////////////////////////////////////
//            T O R S O
// /////////////////////////////////////
class Torso : public RobotPart {
  public:
    Torso(string t, string n, int m, double c, double w, string d, string i, int b, int m_a)
     : RobotPart(t, n, m, c, w, d, i), battery_compartments(b), max_arms(m_a) { }
    Torso()
     : RobotPart(), battery_compartments(), max_arms() { }
    int getBatteryCompartments() { return battery_compartments; }
    int getMaxArms() { return max_arms; }
    string part_to_string();
    string export_part();
    friend ofstream& operator<<(ofstream& ofs, const Torso torso);
    friend istringstream& operator>>(istringstream& is, Torso& torso);
  private:
    int battery_compartments;
    int max_arms;
};
string Torso::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"Battery Compartments : "+std::to_string(battery_compartments)+", Max Arms : "+std::to_string(max_arms)+"\n\n";
}
ofstream& operator<<(ofstream& ofs, const Torso torso) {
  ofs << torso.export_data()+'|'+to_string(torso.battery_compartments)+'|'+to_string(torso.max_arms);
  return ofs;
}
istringstream& operator>>(istringstream& is, Torso& torso) {
  string _type, _name, _model_number, _cost, _weight, _description, _image_filename, _battery_compartments, _max_arms;
  _type = "Torso";
  int delcount = 1;
  for(char c; is.get(c);) {
    if(c == '|') {
      delcount++;
    } else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _name += c;
    } else if(delcount == 2 && isdigit(c)) {
      _model_number += c;
    } else if(delcount == 3 && (isdigit(c) || c == '.')) {
      _cost += c;
    } else if(delcount == 4 && (isdigit(c) || c == '.')) {
      _weight += c;
    } else if(delcount == 5 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _description += c;
    } else if(delcount == 6 && (isalnum(c) || c == '.')) {
      _image_filename += c;
    } else if(delcount == 7 && isdigit(c)) {
      _battery_compartments += c;
    } else if(delcount == 8 && isdigit(c)) {
      _max_arms += c;
    }

    if(delcount > 8) break;
  }
  torso.type = _type;
  torso.name = _name;
  torso.model_number = stoi(_model_number);
  torso.cost = stod(_cost);
  torso.weight = stod(_weight);
  torso.description = _description;
  torso.image_filename = _image_filename;
  torso.battery_compartments = stoi(_battery_compartments);
  torso.max_arms = stoi(_max_arms);
}
// /////////////////////////////////////
//         R O B O T   M O D E L
// /////////////////////////////////////
class RobotModel {
  public:
    RobotModel(string n, int m, RobotPart* t, RobotPart* h, RobotPart* l, vector<RobotPart*> a, vector<RobotPart*> b, int numOfB, int numOfA)
     : name(n), model_number(m), torso(t), head(h), locomotor(l), arms(a), batteries(b), num_of_batteries(numOfB), num_of_arms(numOfA) { }
    RobotModel()
     : name(), model_number(), torso(), head(), locomotor(), arms(), batteries(), num_of_batteries(), num_of_arms() { }
    RobotPart* getTorso() { return torso; }
    RobotPart* getHead() { return head; }
    RobotPart* getLocomotor() { return locomotor; }
    RobotPart* getArm(int index) { return arms[index]; }
    RobotPart* getBattery(int index) { return batteries[index]; }
    string getName() { return name; }
    string getModelNumber() { return std::to_string(model_number); }
    double total_weight();
    double cost_of_parts();
    double max_speed();
    double battery_life();
    string to_string();
    string basic_to_string();
    friend ofstream& operator<<(ofstream& ofs, const RobotModel model);
    friend istringstream& operator>>(istringstream& is, RobotModel& model);
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
  output += dtos(battery_life(), 2)+"[h]\n\n";

  output += torso->part_to_string()+""+head->part_to_string()+""+locomotor->part_to_string();

  for(int i = 0; i < num_of_arms; i++) {
    output += "\tArm "+std::to_string(i+1)+" of "+std::to_string(num_of_arms)+"\n"+arms[i]->part_to_string();
  }
  for(int i = 0; i < num_of_batteries; i++) {
    output += "\tBattery "+std::to_string(i+1)+" of "+std::to_string(num_of_arms)+"\n"+batteries[i]->part_to_string();
  }

  return output;
}
string RobotModel::basic_to_string() {
  string output = "Robot Model : "+name+", Model #"+std::to_string(model_number);
  output += " - $"+dtos(cost_of_parts(), 2)+"\nMax Speed : "+dtos(max_speed(), 2)+" [mph], Max Battery Life : ";
  output += dtos(battery_life(), 2)+"[h]\n\n";
  return output;
}
ofstream& operator<<(ofstream& ofs, const RobotModel model) {
  ofs << model.name+'|'+to_string(model.model_number)+"|";

  ofs << *static_cast<Torso*>(model.torso) << '|';
  ofs << *static_cast<Head*>(model.head) << '|';
  ofs << *static_cast<Locomotor*>(model.locomotor) << '|';

  ofs << model.num_of_arms << '|';
  for(int i = 0; i < model.num_of_arms; i++) {
    ofs << *static_cast<Arm*>(model.arms[i]) << '|';
  }
  ofs << model.num_of_batteries << '|';
  for(int i = 0; i < model.num_of_batteries; i++) {
    ofs << *static_cast<Battery*>(model.batteries[i]) << '|';
  }

  return ofs;
}
istringstream& operator>>(istringstream& is, RobotModel& model) {
  string _name, _model_number, _num_of_batteries, _num_of_arms;
  Torso* torso = new Torso{};
  Head* head = new Head{};
  Locomotor* locomotor = new Locomotor{};
  Arm* arm = new Arm{};
  Battery* battery = new Battery{};
  vector<RobotPart*> arms;
  vector<RobotPart*> batteries;
  int delcount = 1;
  for(char c; is.get(c);) {
    if(c == '|') {
      delcount++;
    } else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _name += c;
    } else if(delcount == 2 && isdigit(c)) {
      _model_number += c;
    } else if(delcount == 3) {
      is.putback(c);
      is >> (*torso);
    } else if(delcount == 4) {
      is.putback(c);
      is >> (*head);
    } else if(delcount == 5) {
      is.putback(c);
      is >> (*locomotor);
    }
    //List of arms
    else if(delcount == 6 && isdigit(c)) {
      _num_of_arms += c;
    } else if(delcount == 7) {
      for(int i = 0; i < stoi(_num_of_arms); i++) {
        is >> (*arm);
        arms.push_back(arm);
      }
    }
    //List of batteries
    else if(delcount == 8 && isdigit(c)) {
      _num_of_batteries += c;
    } else if(delcount == 9) {
      for(int i = 0; i < stoi(_num_of_batteries); i++) {
        is >> (*battery);
        batteries.push_back(battery);
      }
    } else if(delcount > 9) {
      is.putback(c);
    }

    if(delcount > 9) break;
  }
  model.name = _name;
  model.model_number = stoi(_model_number);
  model.torso = torso;
  model.head = head;
  model.locomotor = locomotor;
  model.arms = arms;
  model.batteries = batteries;
}
// /////////////////////////////////////
//    B E L O V E D   C U S T O M E R
// /////////////////////////////////////
class Customer {
  public:
    Customer(string _name, int _customer_num, string _phone_num, string _email);
    Customer()
     : name(), customer_number(), phone_number(), email_address() { }
    string getName() { return name; }
    int getCustomerNumber() { return customer_number; }
    string getPhoneNumber() { return phone_number; }
    string getEmailAddress() { return email_address; }
    string to_string();
    friend ofstream& operator<<(ofstream& ofs, const Customer customer);
    friend istringstream& operator>>(istringstream& is, Customer& customer);
  private:
    string name;
    int customer_number;
    string phone_number;
    string email_address;
};
Customer::Customer(string _name, int _customer_num, string _phone_num, string _email) {
  name = _name;
  customer_number = _customer_num;
  phone_number = _phone_num;
  email_address = _email;
  //This is so that fulltick is able to display test.
  size_t location_of_at = email_address.find_first_of("@");
  email_address.insert(location_of_at, "@");
}
string Customer::to_string() {
  string output = "Customer #"+std::to_string(customer_number)+" - "+name+'\n';
  output += phone_number+", "+email_address+'\n';
  return output;
}
ofstream& operator<<(ofstream& ofs, const Customer customer) {
  ofs << customer.name+'|'+to_string(customer.customer_number)+'|'+customer.phone_number+'|'+customer.email_address;
  return ofs;
}
istringstream& operator>>(istringstream& is, Customer& customer) {
  string _name, _customer_number, _phone_number, _email_address;
  int delcount = 1;
  for(char c; is.get(c);) {
    if(c == '|') {
      delcount++;
    } else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _name += c;
    } else if(delcount == 2 && isdigit(c)) {
      _customer_number += c;
    } else if(delcount == 3 && (isalnum(c) || ispunct(c) || c == '-')) {
      _phone_number += c;
    } else if(delcount == 4 && (isalnum(c) || ispunct(c))) {
      _email_address += c;
    } else if(delcount > 4) {
      is.putback(c);
    }

    if(delcount > 4) break;
  }
  customer.name = _name;
  customer.customer_number = stoi(_customer_number);
  customer.phone_number = _phone_number;
  customer.email_address = _email_address;
}
// /////////////////////////////////////
//     S A L E S   A S S O C I A T E
// /////////////////////////////////////
class SalesAssociate {
  public:
    SalesAssociate(string _name, int _employee_num)
     : name(_name), employee_number(_employee_num) { }
    SalesAssociate()
     : name(), employee_number() { }
    string getName() { return name; }
    int getEmployeeNumber() { return employee_number; }
    string to_string();
    friend ofstream& operator<<(ofstream& ofs, const SalesAssociate salesAssociate);
    friend istringstream& operator>>(istringstream& is, SalesAssociate& associate);
  private:
    string name;
    int employee_number;
};
string SalesAssociate::to_string() {
  string output = "Sales Associate #"+std::to_string(employee_number)+" - "+name+'\n';
  return output;
}
ofstream& operator<<(ofstream& ofs, const SalesAssociate salesAssociate) {
  ofs << salesAssociate.name+'|'+to_string(salesAssociate.employee_number);
  return ofs;
}
istringstream& operator>>(istringstream& is, SalesAssociate& associate) {
  string name, employee_number;
  int delcount = 1;
  for(char c; is.get(c);) {
    if(c == '|') {
      delcount++;
    } else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      name += c;
    } else if(delcount == 2 && isdigit(c)) {
      employee_number += c;
    } else if(delcount > 2) {
      is.putback(c);
    }

    if(delcount > 2) break;
  }
  associate.name = name;
  associate.employee_number = stoi(employee_number);
}
// /////////////////////////////////////
//             O R D E R
// /////////////////////////////////////
class Order {
  public:
    Order(int _order_num, string _date, Customer c, SalesAssociate sa, RobotModel rm, int _status)
     : order_number(_order_num), date(_date), customer(c), salesAssociate(sa), robotModel(rm), status(_status) { }
    Order()
     : order_number(), date(), customer(), salesAssociate(), robotModel(), status() { }
    string to_string();
    friend ofstream& operator<<(ofstream& ofs, const Order order);
    friend istringstream& operator>>(istringstream& is, Order& order);
  private:
    int order_number;
    string date;
    Customer customer;
    SalesAssociate salesAssociate;
    RobotModel robotModel;
    int status;
};
string Order::to_string() {
  string output = "Order #"+std::to_string(order_number)+" - "+date+"\n\n";
  output += customer.to_string()+'\n';
  output += salesAssociate.to_string()+'\n';
  output += robotModel.basic_to_string();
  output += "Order status: "+std::to_string(status);
  return output;
}
ofstream& operator<<(ofstream& ofs, const Order order) {
  ofs << to_string(order.order_number)+'|'+order.date+'|';
  ofs << order.customer << '|';
  ofs << order.salesAssociate << '|';
  ofs << order.robotModel << to_string(order.status);
  return ofs;
}
istringstream& operator>>(istringstream& is, Order& order) {
  string _order_number, _date, _status;
  Customer customer;
  SalesAssociate salesAssociate;
  RobotModel robotModel;
  int delcount = 1;
  for(char c; is.get(c);) {
    if(c == '|') {
      delcount++;
    } else if(delcount == 1 && isdigit(c)) {
      _order_number += c;
    } else if(delcount == 2 && (isalnum(c) || ispunct(c) || c == ' ')) {
      _date += c;
    } else if(delcount == 3) {
      is.putback(c);
      is >> customer;
    } else if(delcount == 4) {
      is.putback(c);
      is >> salesAssociate;
    } else if(delcount == 5) {
      is.putback(c);
      is >> robotModel;
    } else if(delcount == 6 && isdigit(c)) {
      _status += c;
    } else if(delcount > 6) {
      is.putback(c);
    }

    if(delcount > 6) break;
  }
  order.order_number = stoi(_order_number);
  order.date = _date;
  order.customer = customer;
  order.salesAssociate = salesAssociate;
  order.robotModel = robotModel;
  order.status = stoi(_status);
}
// /////////////////////////////////////
//              S H O P
// /////////////////////////////////////
class Shop {
  public:
    void create_new_robot_part(int choice);
    RobotPart* get_part(int index) { return robotparts[index]; }
    string part_to_string(int index);
    string part_list_to_string();
    string part_list_to_string(string type);
    int num_of_parts() { return robotparts.size(); }

    void create_new_robot_model();
    RobotModel get_model(int index) { return robotmodels[index]; }
    string model_to_string(int index);
    string model_list_to_string();
    int num_of_models() { return robotmodels.size(); }

    void create_new_customer();
    Customer get_customer(int index) { return customers[index]; }
    string customer_to_string(int index);
    string customer_list_to_string();
    int num_of_customers() { return customers.size(); }

    void create_new_sales_associate();
    SalesAssociate get_sales_associate(int index) { return salesassociates[index]; }
    string sales_associate_to_string(int index);
    string sales_associate_list_to_string();
    int num_of_sales_associates() { return salesassociates.size(); }

    void create_new_order();
    Order get_order(int index) { return orders[index]; }
    string order_to_string(int index);
    string order_list_to_string();
    int num_of_orders() { return orders.size(); }

    void save(string filename);
    void open(string filename);
    void easter_egg();
  private:
    int get_robot_part(string type);
    int get_customer();
    int get_sales_associate();
    int get_robot_model();

    int find_part(string type, int part_id);
    int find_customer(int customer_id);
    int find_sales_associate(int associate_id);
    int find_robot_model(int model_id);

    vector<RobotPart*> robotparts;
    vector<RobotModel> robotmodels;
    vector<Customer> customers;
    vector<SalesAssociate> salesassociates;
    vector<Order> orders;
};

void Shop::create_new_robot_part(int choice) {
  display_message("Enter information as prompted.", "Gathering default robot part information...");
  string name, description, type, image_filename;
  int model_number;
  double cost, weight;

  name = get_string("Robot Part Creation", "Part Name? ");
  model_number = get_int(name, "Model Number? ");
  cost = get_double(name, "Cost[$]? ");
  weight = get_double(name, "Weight[lbs]? ");
  description = get_string(name, "Description? ");
  image_filename = "default.png";

  RobotPart* part;

  if(choice == 1) {
    display_message("Arm", "Gathering unique robot arm information...");
    double max_power;

    type = "Arm";
    max_power = get_double(name, "Max Power[W]? ");
    part = new Arm{type, name, model_number, cost, weight, description, image_filename, max_power};
    robotparts.push_back(part);

  } else if(choice == 2) {
    display_message("Battery", "Gathering unique battery information...");
    double power_available, max_energy;

    type = "Battery";
    power_available = get_double(name, "Max Power[W]? ");
    max_energy = get_double(name, "Max Energy[kWh]? ");
    part = new Battery{type, name, model_number, cost, weight, description, image_filename, power_available, max_energy};
    robotparts.push_back(part);

  } else if(choice == 3) {
    display_message("Head", "Gathering unique robot head information...");
    double power;

    type = "Head";
    power = get_double(name, "Power? ");
    part = new Head{type, name, model_number, cost, weight, description, image_filename, power};
    robotparts.push_back(part);

  } else if(choice == 4) {
    display_message("Locomotor", "Gathering unique locomotor information...");
    double max_power, max_speed;

    type = "Locomotor";
    max_power = get_double(name, "Max Power[W]? ");
    max_speed = get_double(name, "Max Speed[mph]? ");
    part = new Locomotor{type, name, model_number, cost, weight, description, image_filename, max_power, max_speed};
    robotparts.push_back(part);

  } else if(choice == 5) {
    display_message("Torso", "Gathering unique robot torso information...");
    int battery_compartments, max_arms;

    type = "Torso";
    battery_compartments = get_int(name, "Battery Compartments? ", 1, 3);
    max_arms = get_int(name, "Max Arms? ", 1, 2);
    part = new Torso{type, name, model_number, cost, weight, description, image_filename, battery_compartments, max_arms};
    robotparts.push_back(part);

  }
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
    display_message("Error", "Fatal Error - Less than 5 parts exist in the shop\'s inventory.");
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

  RobotModel model;

  name = get_string(name, "Model Name? ");
  model_number = get_int(name, "Model Number? ");

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
  numOfArms = get_int(name, "How many arms? ", 1, maxArms);

  for(int i = 0; i < numOfArms; i++) {
    type = "Arm";
    partIndex = get_robot_part(type);
    if(partIndex >= 0)
      arms.push_back(robotparts[partIndex]);
    else
      return;
  }

  maxBatteries = static_cast<Torso*>(torso)->getBatteryCompartments();
  numOfBatteries = get_int(name, "How many batteries? ", 1, maxBatteries);

  for(int i = 0; i < numOfBatteries; i++) {
    type = "Battery";
    partIndex = get_robot_part(type);
    if(partIndex >= 0)
      batteries.push_back(robotparts[partIndex]);
    else
      return;
  }

  model = RobotModel{name, model_number, torso, head, locomotor, arms, batteries, numOfBatteries, numOfArms};
  robotmodels.push_back(model);

}
string Shop::model_to_string(int index) {
  return robotmodels[index].to_string();
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
  display_message("Enter information as prompted", "Gathering customer information...");
  string name, phone_number, email_address;
  int customer_number;

  name = get_string(name, "Name[First and Last]? ");
  phone_number = get_string(name, "Phone Number? ");
  email_address = get_string(name, "Email Address? ");
  customer_number = get_int(name, "Customer ID#? ");

  Customer customer = Customer{name, customer_number, phone_number, email_address};
  customers.push_back(customer);

}
string Shop::customer_to_string(int index) {
  return customers[index].to_string();
}
string Shop::customer_list_to_string() {
  if(customers.size() == 0) return "No customer accounts on record.\n";
  string output;
  for(int i = 0; i < customers.size(); i++) {
    output += customer_to_string(i);
  }
  return output;
}

void Shop::create_new_sales_associate() {
  display_message("Enter information as prompted", "Gathering employee information...");
  string name;
  int employee_number;

  name = get_string("Sales Associate Account Creation", "Name[First and Last]? ");
  employee_number = get_int(name, "Employee ID#" );

  SalesAssociate salesAssociate = SalesAssociate{name, employee_number};
  salesassociates.push_back(salesAssociate);
}
string Shop::sales_associate_to_string(int index) {
  return salesassociates[index].to_string();
}
string Shop::sales_associate_list_to_string() {
  if(salesassociates.size() == 0) return "No sales associates on record.\n";
  string output;
  for(int i = 0; i < salesassociates.size(); i++) {
    output += sales_associate_to_string(i);
  }
  return output;
}

void Shop::create_new_order() {
  if(customers.size() == 0) {
    display_message("Error", "Order cannot be created as no customer accounts exist.");
    return;
  }
  if(salesassociates.size() == 0) {
    display_message("Error", "Order cannot be created as no sales associate accounts exist.");
    return;
  }
  if(robotmodels.size() == 0) {
    display_message("Error", "Order cannot be created as no robot models are in the shop.");
  }
  display_message("Enter information as prompted", "Gathering order information...");
  string date;
  int order_number, status, index;
  Customer customer;
  SalesAssociate salesAssociate;
  RobotModel robotModel;

  date = get_string("Order Creation", "Today's Date? ");
  order_number = get_int("Order Creation", "Order ID#? ");
  status = 0;

  index = get_customer();
  customer = customers[index];

  index = get_sales_associate();
  salesAssociate = salesassociates[index];

  index = get_robot_model();
  robotModel = robotmodels[index];

  Order order = Order{order_number, date, customer, salesAssociate, robotModel, status};
  orders.push_back(order);
}
string Shop::order_to_string(int index) {
  return orders[index].to_string();
}
string Shop::order_list_to_string() {
  if(orders.size() == 0) return "No orders on record.";
  string output = "";
  for(int i = 0; i < orders.size(); i++) {
    output += order_to_string(i);
  }
  return output;
}

int Shop::get_robot_part(string type) {
  string partName, prompt;
  bool partExists = false;
  bool partsAvailable = false;
  int partIndex;

  prompt = "Select a part.\n"+type+" name? ";

  display_message(type, "Accessing "+type+" information...");

  while(true) {
    display_message(type+" list", part_list_to_string(type));

    partName = get_string("Part Selection", prompt);
    for(int i = 0; i < robotparts.size(); i++) {
      if(robotparts[i]->getType() == type) partsAvailable = true;
      if((robotparts[i]->getType() == type) & (robotparts[i]->getName() == partName)) {
        partExists = true;
        partIndex = i;
      }
    }

    if(!partsAvailable) break;
    if(partExists & partsAvailable) break;
    display_message("Error", "Please re-enter part name.");
  }

  if(!partsAvailable) {
    display_message("Error", "Fatal - No "+type+" parts available.");
    return -1;
  }
  return partIndex;
}
int Shop::get_customer() {
  string customerName, prompt;
  bool customerExists;
  int customerIndex;

  prompt = "Please select a customer account. ";

  display_message("Customer list", "Retrieving customer account information...");
  while(true) {
    display_message("Customer list", customer_list_to_string());

    customerName = get_string("Customer Account Selection", prompt);
    for(int i = 0; i < customers.size(); i++) {
      if(customers[i].getName() == customerName) {
        customerExists == true;
        customerIndex = i;
      }
    }

    if(customerExists) break;
    display_message("Error", "Please re-enter customer name.");
  }
  return customerIndex;
}
int Shop::get_sales_associate() {
  string associateName, prompt;
  bool associateExists;
  int associateIndex;

  prompt = "Which sales associate helped you? ";

  display_message("Sales Associate list", "Retrieving sales associate information...");
  while(true) {
    display_message("Sales Associate list", sales_associate_list_to_string());

    associateName = get_string("Associate Selection", prompt);
    for(int i = 0; i < salesassociates.size(); i++) {
      if(salesassociates[i].getName() == associateName) {
        associateExists = true;
        associateIndex = i;
      }
    }

    if(associateExists) break;
    display_message("Error", "Error - Please re-enter associate name.");
  }
  return associateIndex;
}
int Shop::get_robot_model() {
  string modelName, prompt;
  bool modelExists;
  int modelIndex;

  prompt = "Which robot model would you like to purchase? ";

  display_message("Robot Model list", "Retrieving robot model information...");
  while(true) {
    display_message("Robot Model list", model_list_to_string());

    modelName = get_string("Robot Model Selection", prompt);
    for(int i = 0; i < robotmodels.size(); i++) {
      if(robotmodels[i].getName() == modelName) {
        modelExists = true;
        modelIndex = i;
      }
    }

    if(modelExists) break;
    display_message("Error", "Error - Please re-enter model name.");
  }

  return modelIndex;
}
void Shop::easter_egg() {
  display_message("Easter Egg", "Filling databases for testing...");
  string image_filename = "image.png";
  RobotPart* arm;
  RobotPart* battery;
  RobotPart* head;
  RobotPart* locomotor;
  RobotPart* torso;

  vector<RobotPart*> arms;
  vector<RobotPart*> batteries;

  RobotModel model;
  Customer customer;
  SalesAssociate associate;
  Order order;

  arm = new Arm{"Arm", "ACME Arm", 5431, 49.95, 15, "Standard Issue", image_filename, 8999};
  robotparts.push_back(arm);
  arms.push_back(arm);
  arms.push_back(arm);
  battery = new Battery{"Battery", "ACME Battery", 9627, 14.95, 0.5, "Standard Issue", image_filename, 12000, 56000};
  robotparts.push_back(battery);
  batteries.push_back(battery);
  batteries.push_back(battery);

  head = new Head{"Head", "ACME Head", 1625, 149.95, 25, "Standard Issue", image_filename, 8999};
  robotparts.push_back(head);
  locomotor = new Locomotor{"Locomotor", "ACME Locomotor", 1830, 249.95, 75.5, "Standard Issue", image_filename, 8999, 500.5};
  robotparts.push_back(locomotor);
  torso = new Torso{"Torso", "ACME Torso", 1151, 99.95, 120, "Standard Issue", image_filename, 2, 2};
  robotparts.push_back(torso);

  model = RobotModel{"ACME Robo", 5843, torso, head, locomotor, arms, batteries, 2, 2};
  robotmodels.push_back(model);

  customer = Customer{"John Smith", 2448, "817-555-5555", "email@aol.com"};
  customers.push_back(customer);

  associate = SalesAssociate{"David Williams", 4562};
  salesassociates.push_back(associate);

  order = Order{999, "January 2, 1997", customer, associate, model, 1};
  orders.push_back(order);

}

void Shop::save(string filename) {
  ofstream file (filename);
  if(file.is_open()) {
    //RobotPart vector
    string type;
    for(int i = 0; i < robotparts.size(); i++) {
      type = robotparts[i]->getType();
        if(type == "Arm") {
          file << "a";
          file << *static_cast<Arm*>(robotparts[i]) << '\n';
        } else if(type == "Battery") {
          file << "b";
          file << *static_cast<Battery*>(robotparts[i]) << '\n';
        } else if(type == "Head") {
          file << "h";
          file << *static_cast<Head*>(robotparts[i]) << '\n';
        } else if(type == "Locomotor") {
          file << "l";
          file << *static_cast<Locomotor*>(robotparts[i]) << '\n';
        } else if(type == "Torso") {
          file << "t";
          file << *static_cast<Torso*>(robotparts[i]) << '\n';
        }
    }
    //RobotModel vector
    for(int i = 0; i < robotmodels.size(); i++) {
      file << "r";
      file << robotmodels[i] << '\n';
    }
    //Customer vector
    for(int i = 0; i < customers.size(); i++) {
      file << "c";
      file << customers[i] << '\n';
    }
    //SalesAssociate vector
    for(int i = 0; i < salesassociates.size(); i++) {
      file << "s";
      file << salesassociates[i] << '\n';
    }
    //Order vector
    for(int i = 0; i < orders.size(); i++) {
      file << "o";
      file << orders[i] << '\n';
    }
  }
  else
    display_message("Error", "Unable to open file");
  file.close();
  return;
}
void Shop::open(string filename) {
  //Variables to be used while resolving shop data
  string line;
  char type;
  ifstream file (filename);

  //Early return if file is unopenable
  if(file.is_open()){
    display_message(filename, "Opening file...");
  } else {
    display_message("Error", "File is not working...");
    return;
  }
  while(getline(file, line)) {
    istringstream ss (line);
    type = ss.get();
    //Resolving RobotPart list data to vector elements
    if(type == 'a') {
      Arm* arm = new Arm{};
      ss >> (*arm);
      robotparts.push_back(arm);
    } else if(type == 'b') {
      Battery* battery = new Battery{};
      ss >> (*battery);
      robotparts.push_back(battery);
    } else if(type == 'h') {
      Head* head = new Head{};
      ss >> (*head);
      robotparts.push_back(head);
    } else if(type == 'l') {
      Locomotor* locomotor = new Locomotor{};
      ss >> (*locomotor);
      robotparts.push_back(locomotor);
    } else if(type == 't') {
      Torso* torso = new Torso{};
      ss >> (*torso);
      robotparts.push_back(torso);
    }
    //Resolving RobotModel list data to vector elements
    else if(type == 'r') {
      RobotModel model;
      ss >> model;
      robotmodels.push_back(model);
    }
    //Resolving Customer list data to vector elements
    else if(type == 'c') {
      Customer customer;
      ss >> customer;
      customers.push_back(customer);
    }
    //Resolving SalesAssociate list data to vector elements
    else if(type == 's') {
      SalesAssociate associate;
      ss >> associate;
      salesassociates.push_back(associate);
    }
    //Resolving Order list data to vector elements
    else if(type == 'o') {
      Order order;
      ss >> order;
      orders.push_back(order);
    }
  }
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
    string get_sales_associate_list();
    string get_order_list();

    string get_help();
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
  3 Save
  4 Open
  0 Exit Program

  )";
  return menu;
}
string View::get_create_menu() {
  string menu =
  R"(
  Create
  ------
  1 Order
  2 Customer
  3 Sales Associate
  4 Part
  5 Model
  6 Easter Egg
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
  1 Order
  2 Customer
  3 Sales Associate
  4 Part
  5 Model
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
string View::get_sales_associate_list() {
  return shop.sales_associate_list_to_string();
}
string View::get_order_list() {
  return shop.order_list_to_string();
}

string View::get_help() {
  return "Here.";
}
// /////////////////////////////////////
//         C O N T R O L L E R
// /////////////////////////////////////

// globel
Fl_Window *win;
Fl_Menu_Bar *menubar;
Shop shop;
View view{shop};

// C a l l b a c k s
void openCB(Fl_Widget* w, void* p) {
  display_message("Load", "Importing data...");
  shop.open("shop.txt");
}
void saveCB(Fl_Widget* w, void* p) {
  display_message("Save", "Exporting data...");
  shop.save("shop.txt");
}
void save_asCB(Fl_Widget* w, void* p) {
  string filename = get_string("Save As", "Enter a filename.");
  display_message("Save As", "Exporting data to "+filename);
  shop.save(filename);
}
void exitCB(Fl_Widget* w, void* p) {
  display_message("Goodbye!", "Exitting program...");
	exit(0);
}

void new_orderCB(Fl_Widget* w, void* p) {
  display_message("New Order", "Initializing order...");
  shop.create_new_order();
}
void new_customerCB(Fl_Widget* w, void* p) {
  display_message("New Customer", "Initializing customer account...");
  shop.create_new_customer();
}
void new_associateCB(Fl_Widget* w, void* p) {
  display_message("New Associate", "Initializing sales associate record...");
  shop.create_new_sales_associate();
}
void new_armCB(Fl_Widget* w, void* p) {
  shop.create_new_robot_part(1);
}
void new_batteryCB(Fl_Widget* w, void* p) {
  shop.create_new_robot_part(2);
}
void new_headCB(Fl_Widget* w, void* p) {
  shop.create_new_robot_part(3);
}
void new_locomotorCB(Fl_Widget* w, void* p) {
  shop.create_new_robot_part(4);
}
void new_torsoCB(Fl_Widget* w, void* p) {
  shop.create_new_robot_part(5);
}
void new_modelCB(Fl_Widget* w, void* p) {
  display_message("New Model", "Initializing robot model...");
  shop.create_new_robot_model();
}

void list_ordersCB(Fl_Widget* w, void* p) {
  display_message("Order List", view.get_order_list());
}
void list_customersCB(Fl_Widget* w, void* p) {
  display_message("Customer Account List", view.get_customer_list());
}
void list_associatesCB(Fl_Widget* w, void* p) {
  display_message("Sales Associate List", view.get_sales_associate_list());
}
void list_partsCB(Fl_Widget* w, void* p) {
  display_message("Robot Part List", view.get_part_list());
}
void list_modelsCB(Fl_Widget* w, void* p) {
  display_message("Robot Model List", view.get_model_list());
}

void helpCB(Fl_Widget* w, void* p) {
  display_message("Help", "H", view.get_help());
}
void eggCB(Fl_Widget* w, void* p) {
	shop.easter_egg();
}

// M e n u
Fl_Menu_Item menuitems[] = {
  { "&File", 0, 0, 0, FL_SUBMENU },
    { "&Open", FL_ALT + 'p', (Fl_Callback *)openCB },
    { "&Save", FL_ALT + 's', (Fl_Callback *)saveCB },
    { "&Save As", 0, (Fl_Callback *)save_asCB },
    { "&Exit", FL_ALT + 'x', (Fl_Callback *)exitCB },
    { 0 },
  { "&Create", 0, 0, 0, FL_SUBMENU },
    { "&Order", FL_ALT + 'o', (Fl_Callback *)new_orderCB },
    { "&Customer", FL_ALT + 'c', (Fl_Callback *)new_customerCB },
    { "&Sales Associate", FL_ALT + 's', (Fl_Callback *)new_associateCB },
    { "&Part", 0, 0, 0, FL_SUBMENU },
      { "&Arm", FL_ALT + 'a' , (Fl_Callback *)new_armCB },
      { "&Battery", FL_ALT + 'b' , (Fl_Callback *)new_batteryCB },
      { "&Head", FL_ALT + 'h' , (Fl_Callback *)new_headCB },
      { "&Locomotor", FL_ALT + 'l' , (Fl_Callback *)new_locomotorCB },
      { "&Torso", FL_ALT + 't' , (Fl_Callback *)new_torsoCB },
      { 0 },
    { "&Robot Model", FL_ALT + 'r', (Fl_Callback *)new_modelCB },
    { 0 },
  { "&Report", 0, 0, 0, FL_SUBMENU },
    { "&Order", 0, (Fl_Callback *)list_ordersCB },
    { "&Customer", 0, (Fl_Callback *)list_customersCB },
    { "&Sales Associate", 0, (Fl_Callback *)list_associatesCB },
    { "&Part", FL_ALT + 0, (Fl_Callback *)list_partsCB },
    { "&Robot Model", 0, (Fl_Callback *)list_modelsCB },
    { 0 },
  { "&Utility", 0, 0, 0, FL_SUBMENU },
    { "&Help", FL_ALT + 'h', (Fl_Callback *)helpCB },
    { "&Egg", FL_ALT + 'e', (Fl_Callback *)eggCB },
    { 0 },
  { 0 }
};

int main() {
  fl_message_hotspot(0);
  const int X = 640;
	const int Y = 480;
	win = new Fl_Window{X, Y, "Robo Boi"};
  win->begin();

    menubar = new Fl_Menu_Bar(0, 0, X, 30);
    menubar->menu(menuitems);

  win->end();
  win->show();
  return(Fl::run());
}
