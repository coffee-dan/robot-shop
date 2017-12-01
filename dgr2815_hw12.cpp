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
		result = stod(fl_input("%s", 0, prompt.c_str()));
    if (0 < result) break;
		fl_message_title("Invalid input");
    fl_message_icon()->label("!");
    fl_message("%s", error.c_str());
  }
  return result;
}
double get_double(string title, string prompt, double min_double) {
  string error = "Please enter a value greater than "+std::to_string(min_double);
  double result;
  while(true) {
		fl_message_title(title.c_str());
    fl_message_icon()->label("?");
		result = stod(fl_input("%s", 0, prompt.c_str()));
    if (min_double < result) break;
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
//-----------------------------------------------------------R O B O T   P A R T
class RobotPart {
  public:
    RobotPart(string t, string n, string m, double c, double w, string d, string i)
     : type(t), name(n), partNumber(m), cost(c), weight(w), description(d), imageFilename(i) { }
    RobotPart()
     : type(), name(), partNumber(), cost(), description(), imageFilename() { }

    string get_type() { return type; }
    string get_name() { return name; }
    string get_part_number() { return partNumber; }
    double get_cost() { return cost; }
    double get_weight() { return weight; }
    string to_string();
    string export_data() const;

    virtual string part_to_string()=0;
    virtual string part_export_data()=0;
  protected:
    string type;
    string name;
    string partNumber;
    double cost;
    double weight;
    string description;
    string imageFilename;
};
string RobotPart::to_string() {
  string output = name+", Model #"+partNumber+" - Cost : $"+dtos(cost, 2);
  output += "\nDescription : "+description+", Weight : "+dtos(weight, 2)+" [lbs]\n";
  return output;
}
string RobotPart::export_data() const{
  return type+'|'+name+'|'+partNumber+'|'+std::to_string(cost)+'|'+std::to_string(weight)+'|'+description+'|'+imageFilename;
}
//-------------------------------------------------------------------------A R M
class Arm : public RobotPart {
  public:
    Arm(string t, string n, string m, double c, double w, string d, string i, double m_p)
     : RobotPart(t, n, m, c, w, d, i), power(m_p) { }
    Arm()
     : RobotPart(), power() { }

    double get_power() { return power; }
    string part_to_string();
    string part_export_data();

    friend istringstream& operator>>(istringstream& is, Arm& arm);
  private:
    double power;
};
string Arm::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output+"Max Power : "+dtos(power, 2)+" [W]\n\n";
}
string Arm::part_export_data() {
  string output = RobotPart::export_data();
  return output+'|'+std::to_string(power);
}
istringstream& operator>>(istringstream& is, Arm& arm) {
  string type, name, partNumber, cost, weight, description, imageFilename, power;
  int delcount = 0;
  for(char c; is.get(c);) {
    if(c == '|') delcount++;
    else if(delcount == 0 && isalnum(c)) {
      type += c;
    }
    else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      name += c;
    }
    else if(delcount == 2 && isdigit(c)) {
      partNumber += c;
    }
    else if(delcount == 3 && (isdigit(c) || c == '.')) {
      cost += c;
    }
    else if(delcount == 4 && (isdigit(c) || c == '.')) {
      weight += c;
    }
    else if(delcount == 5 && (isalnum(c) || ispunct(c) || c == ' ')) {
      description += c;
    }
    else if(delcount == 6 && (isalnum(c) || c == '.')) {
      imageFilename += c;
    }
    else if(delcount == 7 && (isdigit(c) || c == '.')) {
      power += c;
    }
  }
  arm.type = type;
  arm.name = name;
  arm.partNumber = partNumber;
  arm.cost = stod(cost);
  arm.weight = stod(weight);
  arm.description = description;
  arm.power = stod(power);
  arm.imageFilename = imageFilename;
}
//-----------------------------------------------------------------B A T T E R Y
class Battery : public RobotPart {
  public:
    Battery(string t, string n, string m, double c, double w, string d, string i, double se)
     : RobotPart(t, n, m, c, w, d, i), storedEnergy(se) { }
    Battery()
     : RobotPart(), storedEnergy() { }

    double get_stored_energy() { return storedEnergy; }
    string part_to_string();
    string part_export_data();

    friend istringstream& operator>>(istringstream& is, Battery& battery);
  private:
    double storedEnergy;
};
string Battery::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"Stored Energy : "+dtos(storedEnergy, 2)+" [kWh]\n\n";
}
string Battery::part_export_data() {
  string output = RobotPart::export_data();
  return output+'|'+std::to_string(storedEnergy);
}
istringstream& operator>>(istringstream& is, Battery& battery) {
  string type, name, partNumber, cost, weight, description, imageFilename, storedEnergy;
  int delcount = 0;
  for(char c; is.get(c);) {
    if(c == '|') delcount++;
    else if(delcount == 0 && isalnum(c)) {
      type += c;
    }
    else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      name += c;
    }
    else if(delcount == 2 && isdigit(c)) {
      partNumber += c;
    }
    else if(delcount == 3 && (isdigit(c) || c == '.')) {
      cost += c;
    }
    else if(delcount == 4 && (isdigit(c) || c == '.')) {
      weight += c;
    }
    else if(delcount == 5 && (isalnum(c) || ispunct(c) || c == ' ')) {
      description += c;
    }
    else if(delcount == 6 && (isalnum(c) || c == '.')) {
      imageFilename += c;
    }
    else if(delcount == 7 && (isdigit(c) || c == '.')) {
      storedEnergy += c;
    }
  }
  battery.type = type;
  battery.name = name;
  battery.partNumber = partNumber;
  battery.cost = stod(cost);
  battery.weight = stod(weight);
  battery.description = description;
  battery.imageFilename = imageFilename;
  battery.storedEnergy = stod(storedEnergy);
}
//-----------------------------------------------------------------------H E A D
class Head : public RobotPart {
  public:
    Head(string t, string n, string m, double c, double w, string d, string i, double p)
     : RobotPart(t, n, m, c, w, d, i), power(p) { }
    Head()
     : RobotPart(), power() { }

    double get_power() { return power; }
    string part_to_string();
    string part_export_data();

    friend ofstream& operator<<(ofstream& ofs, const Head head);
    friend istringstream& operator>>(istringstream& is, Head& head);
  private:
    double power;
};
string Head::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"Power : "+dtos(power, 2)+" [W]\n\n";
}
string Head::part_export_data() {
  string output = RobotPart::export_data();
  return output+'|'+std::to_string(power);
}
istringstream& operator>>(istringstream& is, Head& head) {
  string type, name, partNumber, cost, weight, description, imageFilename, power;
  int delcount = 0;
  for(char c; is.get(c);) {
    if(c == '|') delcount++;
    else if(delcount == 0 && isalnum(c)) {
      type += c;
    }
    else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      name += c;
    }
    else if(delcount == 2 && isdigit(c)) {
      partNumber += c;
    }
    else if(delcount == 3 && (isdigit(c) || c == '.')) {
      cost += c;
    }
    else if(delcount == 4 && (isdigit(c) || c == '.')) {
      weight += c;
    }
    else if(delcount == 5 && (isalnum(c) || ispunct(c) || c == ' ')) {
      description += c;
    }
    else if(delcount == 6 && (isalnum(c) || c == '.')) {
      imageFilename += c;
    }
    else if(delcount == 7 && (isdigit(c) || c == '.')) {
      power += c;
    }
  }
  head.type = type;
  head.name = name;
  head.partNumber = partNumber;
  head.cost = stod(cost);
  head.weight = stod(weight);
  head.description = description;
  head.imageFilename = imageFilename;
  head.power = stod(power);
}
//-------------------------------------------------------------L O C O M O T O R
class Locomotor : public RobotPart {
  public:
    Locomotor(string t, string n, string m, double c, double w, string d, string i, double m_p, double m_s)
     : RobotPart(t, n, m, c, w, d, i), power(m_p), maxSpeed(m_s) { }
    Locomotor()
     : RobotPart(), power(), maxSpeed() { }

    double get_power() { return power; }
    double get_max_speed() { return maxSpeed; }
    string part_to_string();
    string part_export_data();

    friend ofstream& operator<<(ofstream& ofs, const Locomotor locomotor);
    friend istringstream& operator>>(istringstream& is, Locomotor& locomotor);
  private:
    double power;
    double maxSpeed;
};
string Locomotor::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"Max Power : "+dtos(power, 2)+" [W], Max Speed : "+dtos(maxSpeed, 2)+" [mph]\n\n";
}
string Locomotor::part_export_data() {
  string output = RobotPart::export_data();
  return output+'|'+std::to_string(power)+'|'+std::to_string(maxSpeed);
}
istringstream& operator>>(istringstream& is, Locomotor& locomotor) {
  string type, name, partNumber, cost, weight, description, imageFilename, power, maxSpeed;
  int delcount = 0;
  for(char c; is.get(c);) {
    if(c == '|') delcount++;
    else if(delcount == 0 && isalnum(c)) {
      type += c;
    }
    else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      name += c;
    }
    else if(delcount == 2 && isdigit(c)) {
      partNumber += c;
    }
    else if(delcount == 3 && (isdigit(c) || c == '.')) {
      cost += c;
    }
    else if(delcount == 4 && (isdigit(c) || c == '.')) {
      weight += c;
    }
    else if(delcount == 5 && (isalnum(c) || ispunct(c) || c == ' ')) {
      description += c;
    }
    else if(delcount == 6 && (isalnum(c) || c == '.')) {
      imageFilename += c;
    }
    else if(delcount == 7 && (isdigit(c) || c == '.')) {
      power += c;
    }
    else if(delcount == 8 && (isdigit(c) || c == '.')) {
      maxSpeed += c;
    }
  }
  locomotor.type = type;
  locomotor.name = name;
  locomotor.partNumber = partNumber;
  locomotor.cost = stod(cost);
  locomotor.weight = stod(weight);
  locomotor.description = description;
  locomotor.imageFilename = imageFilename;
  locomotor.power = stod(power);
  locomotor.maxSpeed = stod(maxSpeed);
}
//---------------------------------------------------------------------T O R S O
class Torso : public RobotPart {
  public:
    Torso(string t, string n, string m, double c, double w, string d, string i, int b, int m_a)
     : RobotPart(t, n, m, c, w, d, i), batteryCompartments(b), maxArms(m_a) { }
    Torso()
     : RobotPart(), batteryCompartments(), maxArms() { }

    int get_battery_compartments() { return batteryCompartments; }
    int get_max_arms() { return maxArms; }
    string part_to_string();
    string export_part();
    string part_export_data();

    friend ofstream& operator<<(ofstream& ofs, const Torso torso);
    friend istringstream& operator>>(istringstream& is, Torso& torso);
  private:
    int batteryCompartments;
    int maxArms;
};
string Torso::part_to_string() {
  string output = RobotPart::to_string();
  return type+" : "+output +"Battery Compartments : "+std::to_string(batteryCompartments)+", Max Arms : "+std::to_string(maxArms)+"\n\n";
}
string Torso::part_export_data() {
  string output = RobotPart::export_data();
  return output+'|'+std::to_string(batteryCompartments)+'|'+std::to_string(maxArms);
}
istringstream& operator>>(istringstream& is, Torso& torso) {
  string type, name, partNumber, cost, weight, description, imageFilename, batteryCompartments, maxArms;
  int delcount = 0;
  for(char c; is.get(c);) {
    if(c == '|') delcount++;
    else if(delcount == 0 && isalnum(c)) {
      type += c;
    }
    else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      name += c;
    }
    else if(delcount == 2 && isdigit(c)) {
      partNumber += c;
    }
    else if(delcount == 3 && (isdigit(c) || c == '.')) {
      cost += c;
    }
    else if(delcount == 4 && (isdigit(c) || c == '.')) {
      weight += c;
    }
    else if(delcount == 5 && (isalnum(c) || ispunct(c) || c == ' ')) {
      description += c;
    }
    else if(delcount == 6 && (isalnum(c) || c == '.')) {
      imageFilename += c;
    }
    else if(delcount == 7 && isdigit(c)) {
      batteryCompartments += c;
    }
    else if(delcount == 8 && isdigit(c)) {
      maxArms += c;
    }
  }
  torso.type = type;
  torso.name = name;
  torso.partNumber = partNumber;
  torso.cost = stod(cost);
  torso.weight = stod(weight);
  torso.description = description;
  torso.imageFilename = imageFilename;
  torso.batteryCompartments = stoi(batteryCompartments);
  torso.maxArms = stoi(maxArms);
}
//---------------------------------------------------------R O B O T   M O D E L
class RobotModel {
  public:
    RobotModel(string n, string mn, RobotPart* t, RobotPart* h, RobotPart* l, vector<RobotPart*> a, vector<RobotPart*> b, double c);
    RobotModel()
     : name{}, modelNumber{}, torso{}, head{}, locomotor{}, arms{}, batteries{} { }

    string get_name() { return name; }
    string get_model_number() const { return modelNumber; }
    string to_string();
    string basic_to_string();

    friend ofstream& operator<<(ofstream& ofs, const RobotModel model);
  private:
    string name;
    string modelNumber;
    RobotPart* torso;
    RobotPart* head;
    RobotPart* locomotor;
    vector<RobotPart*> arms;
    vector<RobotPart*> batteries;
    double cost;

    double totalWeight;
    double costOfParts;
    bool powerLimited;
    double batteryLife;
    double maxSpeed;
};
RobotModel::RobotModel(string n, string mn, RobotPart* t, RobotPart* h, RobotPart* l, vector<RobotPart*> a, vector<RobotPart*> b, double c) {
  name = n; modelNumber = mn;
  torso = t; head = h; locomotor = l;
  arms = a; batteries = b;
  cost = c;
  //Total Weight
  double weight = torso->get_weight() + head->get_weight();
  weight += locomotor->get_weight();
  for(int i = 0; i < arms.size(); i++) {
    weight += arms[i]->get_weight();
  }
  for(int i = 0; i < batteries.size(); i++) {
    weight += batteries[i]->get_weight();
  }
  totalWeight = weight;
  //Cost of Parts
  double part_cost = torso->get_cost() + head->get_cost();
  part_cost += locomotor->get_cost();
  for(int i = 0; i < arms.size(); i++) {
    part_cost += arms[i]->get_cost();
  }
  for(int i = 0; i < batteries.size(); i++) {
    part_cost += batteries[i]->get_cost();
  }
  costOfParts = part_cost;
  //Max Speed
  double rated_weight = 5*(locomotor->get_weight());
  double rated_speed = static_cast<Locomotor*>(locomotor)->get_max_speed();
  if(rated_weight > totalWeight)
    maxSpeed = rated_speed*(rated_weight/totalWeight);
  else
    maxSpeed = rated_speed;
  //Batter Life
  double power_consumption = static_cast<Head*>(head)->get_power();
  double arm_power = 0;
  for(int i = 0; i < arms.size(); i++) {
    arm_power += static_cast<Arm*>(arms[i])->get_power();
  }
  power_consumption += 0.40 * arm_power;
  power_consumption += 0.15 * static_cast<Locomotor*>(locomotor)->get_power();

  double model_energy;
  for(int i = 0; i < batteries.size(); i++) {
    model_energy += static_cast<Battery*>(batteries[i])->get_stored_energy();
  }
  model_energy *= 1000; //From kWh to Wh

  batteryLife = model_energy/power_consumption;
}
string RobotModel::to_string() {
  string output = "Robot Model : "+name+", Model #"+modelNumber;
  output += " - Cost : $"+dtos(cost, 2)+"\nMax Speed : "+dtos(maxSpeed, 2)+" [mph], Max Battery Life : ";
  output += dtos(batteryLife, 2)+"[h]\n\n";

  output += "Cost of parts : $"+dtos(costOfParts, 2)+"\n";
  output += torso->part_to_string()+""+head->part_to_string()+""+locomotor->part_to_string();

  for(int i = 0; i < arms.size(); i++) {
    output += "\tArm "+std::to_string(i+1)+" of "+std::to_string(arms.size())+"\n"+arms[i]->part_to_string();
  }
  for(int i = 0; i < batteries.size(); i++) {
    output += "\tBattery "+std::to_string(i+1)+" of "+std::to_string(arms.size())+"\n"+batteries[i]->part_to_string();
  }

  return output;
}
string RobotModel::basic_to_string() {
  string output = "Robot Model : "+name+", Model #"+modelNumber;
  output += " - $"+dtos(cost, 2)+"\nMax Speed : "+dtos(maxSpeed, 2)+" [mph], Max Battery Life : ";
  output += dtos(batteryLife, 2)+"[h]\n\n";
  return output;
}
ofstream& operator<<(ofstream& ofs, const RobotModel model) {
  ofs << 'm' + model.name + '|' + model.modelNumber;
  ofs << '|' + std::to_string(model.cost);

  ofs << '|' + model.torso->get_part_number();
  ofs << '|' + model.head->get_part_number();
  ofs << '|' + model.locomotor->get_part_number();

  for(int i = 0; i < model.arms.size(); i++) {
    ofs << "|!" + model.arms[i]->get_part_number();
  }
  for(int i = 0; i < model.batteries.size(); i++) {
    ofs << "|?" + model.batteries[i]->get_part_number();
  }

  return ofs;
}
//-----------------------------------------------B E L O V E D   C U S T O M E R
class Customer {
  public:
    Customer(string n, string cn, string pn, string e);
    Customer()
     : name{}, customerNumber{}, phoneNumber{}, emailAddress{} { }
    string get_name() { return name; }
    string get_customer_number() const { return customerNumber; }
    string to_string();
    friend ofstream& operator<<(ofstream& ofs, const Customer customer);
    friend istringstream& operator>>(istringstream& is, Customer& customer);
  private:
    string name;
    string customerNumber;
    string phoneNumber;
    string emailAddress;
};
Customer::Customer(string n, string cn, string pn, string e) {
  name = n;
  customerNumber = cn;
  phoneNumber = pn;
  emailAddress = e;
  //This is so that fulltick is able to display text.
  size_t location_of_at = emailAddress.find_first_of("@");
  emailAddress.insert(location_of_at, "@");
}
string Customer::to_string() {
  string output = "Customer #"+customerNumber+" - "+name+'\n';
  output += phoneNumber+", "+emailAddress+'\n';
  return output;
}
ofstream& operator<<(ofstream& ofs, const Customer customer) {
  ofs << 'c'+customer.name+'|'+customer.customerNumber+'|'+customer.phoneNumber+'|'+customer.emailAddress;
  return ofs;
}
istringstream& operator>>(istringstream& is, Customer& customer) {
  string name, customerNumber, phoneNumber, emailAddress;
  int delcount = 1;
  for(char c; is.get(c);) {
    if(c == '|') {
      delcount++;
    } else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      name += c;
    } else if(delcount == 2 && isdigit(c)) {
      customerNumber += c;
    } else if(delcount == 3 && (isalnum(c) || ispunct(c) || c == '-')) {
      phoneNumber += c;
    } else if(delcount == 4 && (isalnum(c) || ispunct(c))) {
      emailAddress += c;
    } else if(delcount > 4) {
      is.putback(c);
    }

    if(delcount > 4) break;
  }
  customer.name = name;
  customer.customerNumber = customerNumber;
  customer.phoneNumber = phoneNumber;
  customer.emailAddress = emailAddress;
}
//-------------------------------------------------S A L E S   A S S O C I A T E
class SalesAssociate {
  public:
    SalesAssociate(string n, string en)
     : name{n}, employeeNumber{en} { }
    SalesAssociate()
     : name{}, employeeNumber{} { }
    string get_name() { return name; }
    string get_employee_number() const { return employeeNumber; }
    string to_string();
    friend ofstream& operator<<(ofstream& ofs, const SalesAssociate salesAssociate);
    friend istringstream& operator>>(istringstream& is, SalesAssociate& associate);
  private:
    string name;
    string employeeNumber;
};
string SalesAssociate::to_string() {
  string output = "Sales Associate #"+employeeNumber+" - "+name+'\n';
  return output;
}
ofstream& operator<<(ofstream& ofs, const SalesAssociate salesAssociate) {
  ofs << 's'+salesAssociate.name+'|'+salesAssociate.employeeNumber;
  return ofs;
}
istringstream& operator>>(istringstream& is, SalesAssociate& associate) {
  string name, employeeNumber;
  int delcount = 1;
  for(char c; is.get(c);) {
    if(c == '|') {
      delcount++;
    } else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
      name += c;
    } else if(delcount == 2 && isdigit(c)) {
      employeeNumber += c;
    } else if(delcount > 2) {
      is.putback(c);
    }

    if(delcount > 2) break;
  }
  associate.name = name;
  associate.employeeNumber = employeeNumber;
}
//---------------------------------------------------------------------O R D E R
class Order {
  public:
    Order(string on, string d, Customer c, SalesAssociate sa, RobotModel rm, string s)
     : orderNumber{on}, date{d}, customer{c}, salesAssociate{sa}, robotModel{rm}, status{s} { }
    Order()
     : orderNumber(), date(), customer(), salesAssociate(), robotModel(), status() { }
    string get_sa_number() const { return salesAssociate.get_employee_number(); }
    string to_string();
    friend ofstream& operator<<(ofstream& ofs, const Order order);
  private:
    string orderNumber;
    string date;
    Customer customer;
    SalesAssociate salesAssociate;
    RobotModel robotModel;
    string status;
};
string Order::to_string() {
  string output = "Order #"+orderNumber+" - "+date+"\n\n";
  output += customer.to_string()+'\n';
  output += salesAssociate.to_string()+'\n';
  output += robotModel.basic_to_string();
  if(status == "0")
    output += "Order status: Pending";
  else if(status == "1")
    output += "Order status: Packaging";
  else if(status == "2")
    output += "Order status: Shipping";
  else if(status == "3")
    output += "Order status: Billing";
  else if(status == "4")
    output += "Order status: Accepting payment";
  else if(status == "5")
    output += "Order status: Completed";
  else if(status == "6")
    output += "Order status: Canceled";
  return output;
}
ofstream& operator<<(ofstream& ofs, const Order order) {
  ofs << 'o'+order.orderNumber+'|'+order.date+'|';
  ofs << order.customer.get_customer_number() << '|';
  ofs << order.salesAssociate.get_employee_number() << '|';
  ofs << order.robotModel.get_model_number() << '|';
  ofs << order.status;
  return ofs;
}
//-----------------------------------------------------------------------S H O P
class Shop {
  public:
    void create_new_robot_part(int choice);
    string part_to_string(int index);
    string get_part_list();
    string get_part_list(string type);

    void create_new_robot_model();
    string model_to_string(int index);
    string get_model_list();
    string get_basic_model_list();

    void create_new_customer();
    string customer_to_string(int index);
    string get_customer_list();

    void create_new_sales_associate();
    string associate_to_string(int index);
    string get_associate_list();

    void create_new_order();
    string order_to_string(int index);
    string get_order_list();
    string get_order_list_by_associate();

    void save(string filename);
    void open(string filename);
    void easter_egg();
  private:
    int get_robot_part(string type);
    int get_customer();
    int get_sales_associate(string prompt);
    int get_robot_model();

    vector<RobotPart*> robotParts;
    vector<RobotModel> robotModels;
    vector<Customer> customers;
    vector<SalesAssociate> salesAssociates;
    vector<Order> orders;
};

void Shop::create_new_robot_part(int choice) {
  display_message("Enter information as prompted.", "Gathering default robot part information...");
  string name, description, type, imageFilename, partNumber;
  double cost, weight;

  name = get_string("Robot Part Creation", "Part Name? ");
  partNumber = get_string(name, "Model Number? ");
  cost = get_double(name, "Cost[$]? ");
  weight = get_double(name, "Weight[lbs]? ");
  description = get_string(name, "Description? ");
  imageFilename = "default.png";

  RobotPart* part;

  if(choice == 1) {
    display_message("Arm", "Gathering unique robot arm information...");
    double power;

    type = "Arm";
    power = get_double(name, "Max Power[W]? ");
    part = new Arm{type, name, partNumber, cost, weight, description, imageFilename, power};
    robotParts.push_back(part);

  } else if(choice == 2) {
    display_message("Battery", "Gathering unique battery information...");
    double storedEnergy;

    type = "Battery";
    storedEnergy = get_double(name, "Max Energy[kWh]? ");
    part = new Battery{type, name, partNumber, cost, weight, description, imageFilename, storedEnergy};
    robotParts.push_back(part);

  } else if(choice == 3) {
    display_message("Head", "Gathering unique robot head information...");
    double power;

    type = "Head";
    power = get_double(name, "Power? ");
    part = new Head{type, name, partNumber, cost, weight, description, imageFilename, power};
    robotParts.push_back(part);

  } else if(choice == 4) {
    display_message("Locomotor", "Gathering unique locomotor information...");
    double power, maxSpeed;

    type = "Locomotor";
    power = get_double(name, "Max Power[W]? ");
    maxSpeed = get_double(name, "Max Speed[mph]? ");
    part = new Locomotor{type, name, partNumber, cost, weight, description, imageFilename, power, maxSpeed};
    robotParts.push_back(part);

  } else if(choice == 5) {
    display_message("Torso", "Gathering unique robot torso information...");
    int batteryCompartments, maxArms;

    type = "Torso";
    batteryCompartments = get_int(name, "Battery Compartments? ", 1, 3);
    maxArms = get_int(name, "Max Arms? ", 1, 2);
    part = new Torso{type, name, partNumber, cost, weight, description, imageFilename, batteryCompartments, maxArms};
    robotParts.push_back(part);

  }
}
string Shop::part_to_string(int index) {
  return robotParts[index]->part_to_string();
}
string Shop::get_part_list() {
  if(robotParts.size() == 0) return "No parts available\n";
  string output;
  for(int i = 0; i < robotParts.size(); i++) {
    output += part_to_string(i);
  }
  return output;
}
string Shop::get_part_list(string type) {
  if(robotParts.size() == 0) return "No parts available\n";
  string output;
  for(int i = 0; i < robotParts.size(); i++) {
    if(robotParts[i]->get_type() == type)
      output += part_to_string(i);
  }
  return output;
}

void Shop::create_new_robot_model() {
  if(robotParts.size() < 5) {
    display_message("Error", "Fatal Error - Less than 5 parts exist in the shop\'s inventory.");
    return;
  }

  string name, type, modelNumber, prompt;
  int partIndex;
  int maxArms, numOfArms;
  int maxBatteries, numOfBatteries;
  double costOfParts, model_cost;
  RobotPart* torso;
  RobotPart* head;
  RobotPart* locomotor;
  vector<RobotPart*> arms;
  vector<RobotPart*> batteries;

  RobotModel model;

  name = get_string(name, "Model Name? ");
  modelNumber = get_string(name, "Model Number? ");

  type = "Torso";
  partIndex = get_robot_part(type);
  if(partIndex >= 0)
    torso = robotParts[partIndex];
  else
    return;

  type = "Head";
  partIndex = get_robot_part(type);
  if(partIndex >= 0)
    head = robotParts[partIndex];
  else
    return;

  type = "Locomotor";
  partIndex = get_robot_part(type);
  if(partIndex >= 0)
    locomotor = robotParts[partIndex];
  else
    return;

  maxArms = static_cast<Torso*>(torso)->get_max_arms();
  numOfArms = get_int(name, "How many arms? ", 1, maxArms);

  for(int i = 0; i < numOfArms; i++) {
    type = "Arm";
    partIndex = get_robot_part(type);
    if(partIndex >= 0)
      arms.push_back(robotParts[partIndex]);
    else
      return;
  }

  maxBatteries = static_cast<Torso*>(torso)->get_battery_compartments();
  numOfBatteries = get_int(name, "How many batteries? ", 1, maxBatteries);

  for(int i = 0; i < numOfBatteries; i++) {
    type = "Battery";
    partIndex = get_robot_part(type);
    if(partIndex >= 0)
      batteries.push_back(robotParts[partIndex]);
    else
      return;
  }

  costOfParts = torso->get_cost() + head->get_cost();
  costOfParts += locomotor->get_cost();
  for(int i = 0; i < arms.size(); i++) {
    costOfParts += arms[i]->get_cost();
  }
  for(int i = 0; i < batteries.size(); i++) {
    costOfParts += batteries[i]->get_cost();
  }
  prompt = "Cost of parts is $"+std::to_string(costOfParts)+".\nWhat is the price of this robot model?";
  model_cost = get_double(name, prompt, costOfParts);

  model = RobotModel{name, modelNumber, torso, head, locomotor, arms, batteries, model_cost};
  robotModels.push_back(model);
}
string Shop::model_to_string(int index) {
  return robotModels[index].to_string();
}
string Shop::get_model_list() {
  if(robotModels.size() == 0) return "No models available.\n";
  string output;
  for(int i = 0; i < robotModels.size(); i++) {
    output += model_to_string(i);
  }
  return output;
}
string Shop::get_basic_model_list() {
  if(robotModels.size() == 0) return "No models available.\n";
  string output;
  for(int i = 0; i < robotModels.size(); i++) {
    output += robotModels[i].basic_to_string();
  }
  return output;
}

void Shop::create_new_customer() {
  display_message("Enter information as prompted", "Gathering customer information...");
  string name, phoneNumber, emailAddress, customerNumber;

  name = get_string(name, "Name[First and Last]? ");
  phoneNumber = get_string(name, "Phone Number? ");
  emailAddress = get_string(name, "Email Address? ");
  customerNumber = get_string(name, "Customer ID#? ");

  Customer customer = Customer{name, customerNumber, phoneNumber, emailAddress};
  customers.push_back(customer);

}
string Shop::customer_to_string(int index) {
  return customers[index].to_string();
}
string Shop::get_customer_list() {
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
  string employeeNumber;

  name = get_string("Sales Associate Account Creation", "Name[First and Last]? ");
  employeeNumber = get_string(name, "Employee ID#" );

  SalesAssociate salesAssociate = SalesAssociate{name, employeeNumber};
  salesAssociates.push_back(salesAssociate);
}
string Shop::associate_to_string(int index) {
  return salesAssociates[index].to_string();
}
string Shop::get_associate_list() {
  if(salesAssociates.size() == 0) return "No sales associates on record.\n";
  string output;
  for(int i = 0; i < salesAssociates.size(); i++) {
    output += associate_to_string(i);
  }
  return output;
}

void Shop::create_new_order() {
  if(customers.size() == 0) {
    display_message("Error", "Order cannot be created as no customer accounts exist.");
    return;
  }
  if(salesAssociates.size() == 0) {
    display_message("Error", "Order cannot be created as no sales associate accounts exist.");
    return;
  }
  if(robotModels.size() == 0) {
    display_message("Error", "Order cannot be created as no robot models are in the shop.");
  }
  display_message("Enter information as prompted", "Gathering order information...");
  string date, orderNumber, status;
  int index;
  Customer customer;
  SalesAssociate salesAssociate;
  RobotModel robotModel;

  date = get_string("Order Creation", "Today's Date? ");
  orderNumber = get_string("Order Creation", "Order ID#? ");
  status = "0";

  index = get_customer();
  customer = customers[index];

  index = get_sales_associate("Which sales associate helped you?");
  salesAssociate = salesAssociates[index];

  index = get_robot_model();
  robotModel = robotModels[index];

  Order order = Order{orderNumber, date, customer, salesAssociate, robotModel, status};
  orders.push_back(order);
}
string Shop::order_to_string(int index) {
  return orders[index].to_string();
}
string Shop::get_order_list() {
  if(orders.size() == 0) return "No orders on record.";
  string output = "";
  for(int i = 0; i < orders.size(); i++) {
    output += order_to_string(i);
  }
  return output;
}
string Shop::get_order_list_by_associate() {
  if(orders.size() == 0) return "No orders on record";
  if(salesAssociates.size() == 0) return "Search could not be conducted as no sales associate accounts exist.";

  int index = get_sales_associate("Enter which associate you would like to audit.");
  string associateIndex = to_string(index);

  string output = "";
  for(int i = 0; i < orders.size(); i++) {
    if(orders[i].get_sa_number() == associateIndex)
      output += order_to_string(i);
  }
  return output;
}

void Shop::save(string filename) {
  /*Function will either access default "shop.txt" or user-defined file depending
   *on which callback function was called. (saveCB or save_asCB)*/
  ofstream file (filename);
  if(file.is_open()) {
    for(int i = 0; i < robotParts.size(); i++) {
      file << robotParts[i]->part_export_data() << '\n';
    }
    for(int i = 0; i < customers.size(); i++) {
      file << customers[i] << '\n';
    }
    for(int i = 0; i < salesAssociates.size(); i++) {
      file << salesAssociates[i] << '\n';
    }

    file << "~\n";
    for(int i = 0; i < robotModels.size(); i++) {
      file << robotModels[i] << '\n';
    }
    file << "~\n";
    for(int i = 0; i < orders.size(); i++) {
      file << orders[i] << '\n';
    }
  }
  else {
    display_message("Error", "Unable to open file");
    return;
  }
  file.close();
}
void Shop::open(string filename) {
  //Variables to be used while resolving shop data
  string line;
  char type;
  ifstream file (filename);

  //Early return if file is unopenable
  if(file.is_open()) {
    display_message(filename, "Opening file...");
  }
  else {
    display_message("Error", "File is not working...");
    return;
  }
  ///Resolving data that is without dependencies
  while(getline(file, line)) {
    istringstream ss {line};
    type = ss.get();

    //Resolving RobotPart list data to vector elements
    if(type == 'A') {
      Arm* arm = new Arm{}; ss.putback(type);
      ss >> (*arm);
      robotParts.push_back(arm);
    }
    else if(type == 'B') {
      Battery* battery = new Battery{}; ss.putback(type);
      ss >> (*battery);
      robotParts.push_back(battery);
    }
    else if(type == 'H') {
      Head* head = new Head{}; ss.putback(type);
      ss >> (*head);
      robotParts.push_back(head);
    }
    else if(type == 'L') {
      Locomotor* locomotor = new Locomotor{}; ss.putback(type);
      ss >> (*locomotor);
      robotParts.push_back(locomotor);
    }
    else if(type == 'T') {
      Torso* torso = new Torso{}; ss.putback(type);
      ss >> (*torso);
      robotParts.push_back(torso);
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
      salesAssociates.push_back(associate);
    }

    else if(type = '~') break;
  }
  //Resolving RobotModel list data to vector elements
  while(getline(file, line)) {
    istringstream ss {line};
    type = ss.get();

    if(type == 'm') {
      string name, modelNumber, cost;
      RobotPart* torso;
      RobotPart* head;
      RobotPart* locomotor;
      vector<RobotPart*> arms, batteries;

      //Model Numbers for each of the robot model parts
      string torso_mn, head_mn, locomotor_mn;
      vector<string> arms_mn, batteries_mn;
      string atemp, btemp;

      int delcount = 0;
      char c;
      for(char c; ss.get(c);) {
        cout << c;
        if(c == '|') delcount++;
        else if(delcount == 0 && (isalnum(c) || ispunct(c) || c == ' ')) {
          name += c;
        }
        else if(delcount == 1 && isdigit(c)) {
          modelNumber += c;
        }
        else if(delcount == 2 && (isdigit(c) || c == '.')) {
          cost += c;
        }
        else if(delcount == 3 && isdigit(c)) {
          torso_mn += c;
        }
        else if(delcount == 4 && isdigit(c)) {
          head_mn += c;
        }
        else if(delcount == 5 && isdigit(c)) {
          locomotor_mn += c;
        }
        else if(delcount > 5) break;
      }
      ss.putback(c);
      for(int i = 0; i < robotParts.size(); i++) {
        if(robotParts[i]->get_part_number() == torso_mn)
          torso = robotParts[i];
        else if(robotParts[i]->get_part_number() == head_mn)
          head = robotParts[i];
        else if(robotParts[i]->get_part_number() == locomotor_mn)
          locomotor = robotParts[i];
      }
      //Puts model numbers for arms and batteries into vectors
      for(char c; ss.get(c);) {
        if(c == '!') {
          getline(ss, atemp, '|');
          cout << atemp << '\n';
          arms_mn.push_back(atemp);
        }
        else if(c == '?') {
          getline(ss, btemp, '|');
          cout << btemp << '\n';
          batteries_mn.push_back(btemp);
        }
      }
      //Uses stored model numbers to set arm and battery data fields
      for(int i = 0; i < arms_mn.size(); i++) {
        for(int j = 0; j < robotParts.size(); j++) {
          if(robotParts[j]->get_part_number() == arms_mn[i]) {
            arms.push_back(robotParts[j]);
          }
        }
      }
      for(int i = 0; i < batteries_mn.size(); i++) {
        for(int j = 0; j < robotParts.size(); j++) {
          if(robotParts[j]->get_part_number() == batteries_mn[i]) {
            batteries.push_back(robotParts[j]);
          }
        }
      }

      RobotModel model = RobotModel{name, modelNumber, torso, head, locomotor, arms, batteries, stod(cost)};
      robotModels.push_back(model);
    }
    else if(type == '~') break;
  }
  //Resolving Order list data to vector elements
  while(getline(file, line)) {
    istringstream ss {line};
    type = ss.get();

    if(type == 'o') {
      string orderNumber, date, status;
      Customer customer;
      SalesAssociate salesAssociate;
      RobotModel robotModel;
      //ID numbers for customer, salesassociate and robotmodel
      string customerNumber, employeeNumber, modelNumber;
      int delcount = 0;
      char c;
      for(char c; ss.get(c);) {
        if(c == '|') delcount++;
        else if(delcount == 0 && isdigit(c)) {
          orderNumber += c;
        }
        else if(delcount == 1 && (isalnum(c) || ispunct(c) || c == ' ')) {
          date += c;
        }
        else if(delcount == 2 && isdigit(c)) {
          customerNumber += c;
        }
        else if(delcount == 3 && isdigit(c)) {
          employeeNumber += c;
        }
        else if(delcount == 4 && isdigit(c)) {
          modelNumber += c;
        }
        else if(delcount == 5 && isdigit(c)) {
          status += c;
        }
        else if(delcount > 6) break;
      }
      //Create boolean values to ensure Order data is not corrupted or malformed
      bool customer_check = false; bool employee_check = false; bool model_check = false;
      for(int i = 0; i < customers.size(); i++) {
        if(customers[i].get_customer_number() == customerNumber) {
          customer = customers[i];
          customer_check = true;
        }
      }
      for(int i = 0; i < salesAssociates.size(); i++) {
        if(salesAssociates[i].get_employee_number() == employeeNumber) {
          salesAssociate = salesAssociates[i];
          employee_check = true;
        }
      }
      for(int i = 0; i < robotModels.size(); i++) {
        if(robotModels[i].get_model_number() == modelNumber) {
          robotModel = robotModels[i];
          model_check = true;
        }
      }
      //If data is correctly formed Order will be added.
      if(customer_check && employee_check && model_check) {
        Order order = Order{orderNumber, date, customer, salesAssociate, robotModel, status};
        orders.push_back(order);
      }
    }
    else if(type = '~') break;

  }
}
void Shop::easter_egg() {
  display_message("Easter Egg", "Filling databases for testing...");
  string imageFilename = "image.png";
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

  arm = new Arm{"Arm", "ACME Arm", "5431", 49.95, 15, "Standard Issue", imageFilename, 8999};
  robotParts.push_back(arm);
  arms.push_back(arm);
  arms.push_back(arm);
  battery = new Battery{"Battery", "ACME Battery", "9627", 14.95, 0.5, "Standard Issue", imageFilename, 56000};
  robotParts.push_back(battery);
  batteries.push_back(battery);
  batteries.push_back(battery);

  head = new Head{"Head", "ACME Head", "1625", 149.95, 25, "Standard Issue", imageFilename, 8999};
  robotParts.push_back(head);
  locomotor = new Locomotor{"Locomotor", "ACME Locomotor", "1830", 249.95, 75.5, "Standard Issue", imageFilename, 8999, 500.5};
  robotParts.push_back(locomotor);
  torso = new Torso{"Torso", "ACME Torso", "1151", 99.95, 120, "Standard Issue", imageFilename, 2, 2};
  robotParts.push_back(torso);

  model = RobotModel{"ACME Robo", "5843", torso, head, locomotor, arms, batteries, 1899.95};
  robotModels.push_back(model);

  customer = Customer{"John Smith", "2448", "817-555-5555", "email@aol.com"};
  customers.push_back(customer);

  associate = SalesAssociate{"David Williams", "4562"};
  salesAssociates.push_back(associate);

  order = Order{"999", "January 2, 1997", customer, associate, model, "1"};
  orders.push_back(order);

}

int Shop::get_robot_part(string type) {
  string partName, prompt;
  bool partExists = false;
  bool partsAvailable = false;
  int partIndex;

  prompt = "Select a part.\n"+type+" name? ";

  display_message(type, "Accessing "+type+" information...");

  while(true) {
    display_message(type+" list", get_part_list(type));

    partName = get_string("Part Selection", prompt);
    for(int i = 0; i < robotParts.size(); i++) {
      if(robotParts[i]->get_type() == type) partsAvailable = true;
      if((robotParts[i]->get_type() == type) & (robotParts[i]->get_name() == partName)) {
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
    display_message("Customer list", get_customer_list());

    customerName = get_string("Customer Account Selection", prompt);
    for(int i = 0; i < customers.size(); i++) {
      if(customers[i].get_name() == customerName) {
        customerExists == true;
        customerIndex = i;
      }
    }

    if(customerExists) break;
    display_message("Error", "Please re-enter customer name.");
  }
  return customerIndex;
}
int Shop::get_sales_associate(string prompt) {
  string associateName;
  bool associateExists;
  int associateIndex;

  display_message("Sales Associate list", "Retrieving sales associate information...");
  while(true) {
    display_message("Sales Associate list", get_associate_list());

    associateName = get_string("Associate Selection", prompt);
    for(int i = 0; i < salesAssociates.size(); i++) {
      if(salesAssociates[i].get_name() == associateName) {
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
    display_message("Robot Model list", get_model_list());

    modelName = get_string("Robot Model Selection", prompt);
    for(int i = 0; i < robotModels.size(); i++) {
      if(robotModels[i].get_name() == modelName) {
        modelExists = true;
        modelIndex = i;
      }
    }

    if(modelExists) break;
    display_message("Error", "Error - Please re-enter model name.");
  }

  return modelIndex;
}
//-----------------------------------------------------------------------V I E W
class View {
  public:
    View(Shop& shop)
     : shop(shop) { }
    string get_help();
  private:
    Shop& shop;
};
string View::get_help() {
  return "Here.";
}

// globel
Fl_Window *win;
Fl_Menu_Bar *menubar;
Shop shop;
View view{shop};

//-------------------------------------------------------------C A L L B A C K S

//FILE
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
//CREATE
void new_orderCB() {
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
//REPORT
void list_ordersCB(Fl_Widget* w, void* p) {
  display_message("Order List", shop.get_order_list());
}
void list_orders_by_associateCB(Fl_Widget* w, void* p) {
  display_message("Order List", shop.get_order_list());
}
void list_customersCB(Fl_Widget* w, void* p) {
  display_message("Customer Account List", shop.get_customer_list());
}
void list_associatesCB(Fl_Widget* w, void* p) {
  display_message("Sales Associate List", shop.get_associate_list());
}
void list_partsCB(Fl_Widget* w, void* p) {
  display_message("Robot Part List", shop.get_part_list());
}
void list_modelsCB(Fl_Widget* w, void* p) {
  display_message("Robot Model List", shop.get_model_list());
}
void list_models_basicCB(Fl_Widget* w, void* p) {
  display_message("Robot Model List", shop.get_basic_model_list());
}
//UTILITY
void helpCB(Fl_Widget* w, void* p) {
  display_message("Help", "H", view.get_help());
}
void manage_orderCB(Fl_Widget* w, void* p) {

}
void eggCB(Fl_Widget* w, void* p) {
	shop.easter_egg();
}

// M e n u
Fl_Menu_Item full_menu[] = {
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
    { "&Order", 0, 0, 0, FL_SUBMENU },
      { "&All Orders", 0, (Fl_Callback *)list_ordersCB },
      { "&Orders by Associate", 0, (Fl_Callback *)list_orders_by_associateCB },
      { 0 },
    { "&Customer", 0, (Fl_Callback *)list_customersCB },
    { "&Sales Associate", 0, (Fl_Callback *)list_associatesCB },
    { "&Part", 0, (Fl_Callback *)list_partsCB },
    { "&Robot Model", 0, 0, 0, FL_SUBMENU },
      { "&Boss List", 0, (Fl_Callback *)list_modelsCB },
      { "&Customer List", 0, (Fl_Callback *)list_models_basicCB },
      { 0 },
    { 0 },
  { "&Utility", 0, 0, 0, FL_SUBMENU },
    { "&Help", FL_ALT + 'h', (Fl_Callback *)helpCB },
    { "&Egg", FL_ALT + 'e', (Fl_Callback *)eggCB },
    { 0 },
  { 0 }
};
//Product Manager View
Fl_Menu_Item manager_menu[] = {
  { "&File", 0, 0, 0, FL_SUBMENU },
    { "&Open", FL_ALT + 'p', (Fl_Callback *)openCB },
    { "&Exit", FL_ALT + 'x', (Fl_Callback *)exitCB },
    { 0 },
  { "&Create", 0, 0, 0, FL_SUBMENU },
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
    { "&Part", 0, (Fl_Callback *)list_partsCB },
    { "&Robot Model", 0, (Fl_Callback *)list_modelsCB },
    { 0 },
  { "&Utility", 0, 0, 0, FL_SUBMENU },
    { "&Help", FL_ALT + 'h', (Fl_Callback *)helpCB },
    { 0 },
  { 0 }
};
//Pointy-haired Boss View
Fl_Menu_Item boss_menu[]  = {
  { "&File", 0, 0, 0, FL_SUBMENU },
    { "&Open", FL_ALT + 'p', (Fl_Callback *)openCB },
    { "&Save", FL_ALT + 's', (Fl_Callback *)saveCB },
    { "&Save As", 0, (Fl_Callback *)save_asCB },
    { "&Exit", FL_ALT + 'x', (Fl_Callback *)exitCB },
    { 0 },
  { "&Create", 0, 0, 0, FL_SUBMENU },
    { "&Customer", FL_ALT + 'c', (Fl_Callback *)new_customerCB },
    { "&Sales Associate", FL_ALT + 's', (Fl_Callback *)new_associateCB },
    { 0 },
  { "&Report", 0, 0, 0, FL_SUBMENU },
    { "&Order", 0, (Fl_Callback *)list_ordersCB },
    { "&Customer", 0, (Fl_Callback *)list_customersCB },
    { "&Sales Associate", 0, (Fl_Callback *)list_associatesCB },
    { "&Robot Part", 0, (Fl_Callback *)list_modelsCB },
    { 0 },
  { "&Utility", 0, 0, 0, FL_SUBMENU },
    { "&Help", FL_ALT + 'h', (Fl_Callback *)helpCB },
    { 0 },
  { 0 }
};
//Sales Associate View
Fl_Menu_Item employee_menu[] = {
  { "&File", 0, 0, 0, FL_SUBMENU },
    { "&Open", FL_ALT + 'p', (Fl_Callback *)openCB },
    { "&Exit", FL_ALT + 'x', (Fl_Callback *)exitCB },
    { 0 },
  { "&Create", 0, 0, 0, FL_SUBMENU },
    { "&Order", FL_ALT + 'o', (Fl_Callback *)new_orderCB },
    { 0 },
  { "&Report", 0, 0, 0, FL_SUBMENU },
    { "&Order", 0, (Fl_Callback *)list_ordersCB },
    { "&Customer", 0, (Fl_Callback *)list_customersCB },
    { "&Sales Associate", 0, (Fl_Callback *)list_associatesCB },
    { "&Robot Model", 0, (Fl_Callback *)list_modelsCB },
    { 0 },
  { "&Utility", 0, 0, 0, FL_SUBMENU },
    { "&Help", FL_ALT + 'h', (Fl_Callback *)helpCB },
    { 0 },
  { 0 }
};
//Beloved Customer View
Fl_Menu_Item customer_menu[] = {
  { "&File", 0, 0, 0, FL_SUBMENU },
    { "&Open", FL_ALT + 'p', (Fl_Callback *)openCB },
    { "&Exit", FL_ALT + 'x', (Fl_Callback *)exitCB },
    { 0 },
  { "&Report", 0, 0, 0, FL_SUBMENU },
    { "&Robot Part", 0, (Fl_Callback *)list_models_basicCB },
    { 0 },
  { "&Utility", 0, 0, 0, FL_SUBMENU },
    { "&Help", FL_ALT + 'h', (Fl_Callback *)helpCB },
    { 0 },
  { 0 }
};
void login() {
  string user_id, user_pass;
  string login_menu = "--User IDs--\npm - Product Manager\npb - Point-haired Boss\nsa - Sales Associate\nbc - Beloved Customer\n\nEnter your User ID.";
  while(true) {
    user_id = get_string(login_menu);
    if((user_id == "pm")||(user_id == "pb")||(user_id == "sa")||(user_id == "bc")||(user_id == "db"))
      break;
    display_message("Error", "Incorrect User ID, please try again!");
  }

  bool successful_login = false;
  while(!successful_login) {
    fl_message_title("Login");
    fl_message_icon()->label("?");
    user_pass = fl_password("Enter your password");
    if((user_id == "pm") && (user_pass == "Capitalism78")) {
      menubar->menu(manager_menu);
      successful_login = true;
    }
    else if((user_id == "pb") && (user_pass == "BestHairInTheOffice11")) {
      menubar->menu(boss_menu);
      successful_login = true;
    }
    else if((user_id == "sa") && (user_pass == "DailyGrind3000")) {
      menubar->menu(employee_menu);
      successful_login = true;
    }
    else if((user_id == "bc") && (user_pass == "Password1234")) {
      menubar->menu(customer_menu);
      successful_login = true;
    }
    else if((user_id == "db") && (user_pass == "segf4ult")) {
      menubar->menu(full_menu);
      successful_login = true;
    }

  }
}

int main() {
  fl_message_hotspot(0);
  Fl_Window beacon (1,1);
  beacon.show();

  const int X = 640;
	const int Y = 480;
	win = new Fl_Window{X, Y, "Robo Boi"};
  win->begin();

    menubar = new Fl_Menu_Bar(0, 0, X, 30);
    login();
    beacon.hide();

  win->end();
  win->show();
  return(Fl::run());
}
