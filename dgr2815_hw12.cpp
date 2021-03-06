/*! \author Daniel Gerard Ramirez
  * \version 2.3
  * \date 2017-12-2
  * \warning This is a student project, do not expect a lot.
  *
  * \mainpage Robbie Robot Shop
  * \section intro_sec Introduction
  * This code is meant to aid a robot vendor in the process of conducting business.
  * \section compile_sec Compilation
  * Here I will describe how to compile this code with GNU make.
  * \subsection Make
  * Navigate to the directory that the file 'dgr2815_hw12.cpp' is located.
  * Input the command $ make
  * To run the executable file that was created, input $ ./hw12 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/fl_ask.H>

using namespace std;

/*! \brief Double to String.
  * \details Converts a double to a string with a precision that is given by the
  * second parameter. */
string dtos(double num, int precision) {
  stringstream ss;
  ss << fixed << setprecision(precision) << num;
  return ss.str();
}
/*! \brief Retrieves input from user via fl_input.
  * \details Takes in title and prompt for the fl_input dialog box that appears
  * requesting a double input from the user. */
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
/*! \brief Version of get_double(string title, string prompt) with minimum input
  * value taken as a parameter.
  * \details Provides ability to retrieve input from user via fl_input with a
  * minimum value being specified such that the user cannot input a lower
  * than min_double. */
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
/*! \brief Retrieves input from user via fl_input
  * \details Takes in title and prompt for the fl_input dialog box that appears
  * requesting an integer input from the user. */
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
/*! \brief Version of get_int(string title, string prompt) with maximum integer
  * value taken as a parameter.
  * \details Provides ability to retrieve input from user via fl_input with a
  * maximum value being specified such that the user cannot input a value higher
  * than max_int. */
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
/*! \brief Version of get_int(string title, string prompt) with both maxmimum and
  * minimum integer values taken as parameters.
  * \details Takes in input via fl_input such that the user is unable to input a
  * value lower than min_int or higher than max_int. */
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
/** \class RobotPart
  *
  * \brief Pure abstract class that has several default data members.
  *
  * RobotPart has seven data members that hold infromation that each of the child
  * classes share with it as they use the RobotModel constructor. */
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
    string type; //!< Contains the RobotPart's type, as a default RobotPart cannot exist.
    string name; //!< RobotPart's name.
    string partNumber; //!< RobotPart's part number.
    double cost; //!< Cost of RobotPart in USD as defined by the product manager.
    double weight; //!< Weight of RobotPart in lbs as defined by the product manager.
    string description; //!< Brief description of the RobotPart.
    string imageFilename; //!< Filename for image of the RobotPart.
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
/** \class Arm
  *
  * \brief Arm is one of five RobotPart types.
  *
  * A RobotModel can have multiple Arm components according the maxArms data
  * member in Torso, another component of RobotModel. Arm only contains one
  * unique data member, power, which represents the amount of power consumed
  * when the RobotModel it is attached to is powered on. */
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
    double power; //!< Power consumed by Arm during user in Watts.
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
/** \class Battery
  *
  * \brief Battery is one of five RobotPart types.
  *
  * Battery has one unique data member, storedEnergy, which represents the
  * amount of energy (in kWh) is within the Battery. This value is used in several
  * calculations within RobotModel. */
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
    double storedEnergy; //!< Energy store in Battery in kWh.
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
/** \class Head
  *
  * \brief Head is one of five RobotPart types.
  *
  * Head only has one unique data member, power, which it shares with Arm and
  * Locomotor. Power defines the amount of power consumed when this RobotModel
  * that this Head is attached to is powered on. */
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
    double power; //!< Power consumed by Head during use in Watts.
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
/** \class Locomotor
  *
  * \brief Locomotor is one of five RobotPart types.
  *
  * Locomotor has the member maxSpeed which is the speed that it can potentially
  * reach so long as it is not supporting more than five times it's own weight.
  * This potential max speed translates to the RobotModel via a calculation that
  * compares five times the Locomotor's weight and the weight of the RobotModel.
  */
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
    double power; //!< Power consumed by Locomotor in Watts.
    double maxSpeed; //!< Rated max speed of Locomotor in mph.
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
/** \class Torso
  *
  * \brief Torso is one of five RobotPart types.
  *
  * Only one Torso can exist on a RobotModel. Torso has two members,
  * batteryCompartments and maxArms that dictate how the maximum Battery and Arm
  * components can be supported by a RobotModel. */
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
    int batteryCompartments; //!< Number of compartments that are available on the Torso.
    int maxArms; //!< Number of Arms that can be attached to the Torso.
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
/** \class RobotModel
  *
  * \brief RobotModel aggregates all five of the RobotPart types.
  *
  * A RobotModel must have one of each RobotPart type in order to be created and
  * added to the Shop's member vector robotModels. RobotModel can have more than
  * one Battery and Arm is the Torso component can support this via members
  * batteryCompartments and maxArms. */
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
    string name; //!< Name of the RobotModel.
    string modelNumber; //!< Model number of the RobotModel, as a string as no calculations are done with this value.
    RobotPart* torso; //!< RobotPart pointer for RobotModel's Torso. Only one can be on a RobotModel.
    RobotPart* head; //!< RobotPart pointer for RobotModel's Head. Only one can be on a RobotModel.
    RobotPart* locomotor; //!< RobotPart pointer for RobotModel's Locomotor. Only one can be on a RobotModel.
    vector<RobotPart*> arms; //!< Vector of RobotPart pointers to store Arm components.
    vector<RobotPart*> batteries; //!< Vector of RobotPart pointers to store Battery components.
    double cost; //!< Cost of RobotModel as defined by the Pointer-Haired Boss.

    double totalWeight; //!< Total weight of RobotModel, calculated when RobotModel is created.
    double costOfParts; //!< Raw cost of all parts combined, to be viewed by Pointy-Haired Boss only.
    bool powerLimited; //!< Boolean value that is true if the total power consumed by Head Torso and Locomotor exceeds that of the Battery's storedEnergy.
    double batteryLife; //!< Estimated battery life of the RobotModel, calculated when RobotModel is created.
    double maxSpeed; //!< Max speed of the RobotModel based on maxSpeed of Locomotor and the rated weight (5 times Locotmotor's weight) of that.
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
/** \class Customer
  *
  * \brief Known as beloved customer, Customer holds information about Shop customers.
  *
  * Customer represents customers of the Shop. The Shop holds records of customers
  * in a member vector. */
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
    string name; //!< Name of Customer account.
    string customerNumber; //!< Unique customer ID number.
    string phoneNumber; //!< Phone number of Customer that is input upon account creation.
    string emailAddress; //!< Email address of Customer that is input upon account creation.
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
/** \class SalesAssociate
  *
  * \brief SalesAssociate holds information about Shop employees.
  *
  * SalesAssociate represents employes of the Shop. The Shop holds records of
  * sales associates in a member vector.
  * */
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
    string name; //!< Name of SalesAssociate.
    string employeeNumber; //!< Unique employee ID number.
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
/** \class Order
  *
  * \brief Order aggregates all other classes except Shop.
  *
  * This class is meant to represent an RobotModel Order. An Order is created by
  * a SalesAssociate for a Customer and includes information about what the customer
  * wishes to purchase from the Shop. Order also keeps track of the status of the
  * Order via a Finite State Machine. */
class Order {
  public:
    Order(string on, string d, Customer c, SalesAssociate sa, vector<RobotModel> ms, string s)
     : orderNumber{on}, date{d}, customer{c}, salesAssociate{sa}, models{ms}, status{s} { }
    Order()
     : orderNumber(), date(), customer(), salesAssociate(), models(), status() { }
    string get_order_number() const { return orderNumber; }
    string get_sa_number() const { return salesAssociate.get_employee_number(); }
    string get_status() const { return status; }
    string to_string();

    void update_status();
    friend ofstream& operator<<(ofstream& ofs, const Order order);
  private:
    string orderNumber; //!< Unique order ID number.
    string date; //!< Date the Order was made.
    Customer customer; //!< Customer who made the order.
    SalesAssociate salesAssociate; //!< SalesAssociate that assisted the Customer.
    vector<RobotModel> models; //!< Vector of RobotModel's that were ordered by the Customer.
    string status;  //!< Current status of the Order, starts at 0.
};
string Order::to_string() {
  string output = "Order #"+orderNumber+" - "+date+"\n";
  output += customer.to_string() + '\n';
  output += salesAssociate.to_string() + '\n';
  for(int i = 0; i < models.size(); i++) {
    output += std::to_string(i+1)+" of "+std::to_string(models.size())+" : "+models[i].basic_to_string();
  }

  if(status == "00")
    output += "Order status: Pending";

  else if(status == "11")
    output += "Order status: Packaging and Billing";
  else if(status == "21")
    output += "Order status: Shipping and Billing";
  else if(status == "31")
    output += "Order status: Billing";
  else if(status == "12")
    output += "Order status: Packaging and Accepting payment";
  else if(status == "13")
    output += "Order status: Packaging";

  else if(status == "22")
    output += "Order status: Shipping and Accepting Payment";
  else if(status == "32")
    output += "Order status: Accepting payment";
  else if(status == "23")
    output += "Order status: Shipping";

  else if(status == "33")
    output += "Order status: Completed";
  else if(status == "44")
    output += "Order status: Canceled";
  return output+"\n\n";
}
void Order::update_status() {
  int update;
  string title = "Order update";
  string prompt = "Select the change you would like to make.\n";
  if(status == "00") {
    prompt += "Begin Order :: 1\n\nCancel Order :: 0\nCancel Update :: -1";
    update = get_int(title, prompt, -1, 1);
    if(update == 1) status = "11";
  }
  else if(status == "11") {
    prompt += "Packaged :: 1\nBilled :: 2\n\nCancel Order :: 0\nCancel Update :: -1";
    update = get_int(title, prompt, -1, 2);
    if(update == 1) status = "21";
    if(update == 2) status = "12";
  }
  else if(status == "12") {
    prompt += "Packaged :: 1\nPaid :: 2\n\nCancel Order :: 0\nCancel Update :: -1";
    update = get_int(title, prompt, -1, 2);
    if(update == 1) status = "22";
    if(update == 2) status = "13";
  }
  else if(status == "21") {
    prompt += "Shipped :: 1\nBilled :: 2\n\nCancel Order :: 0\nCancel Update :: -1";
    update = get_int(title, prompt, -1, 2);
    if(update == 1) status = "31";
    if(update == 2) status = "22";
  }
  else if(status == "22") {
    prompt += "Shipped :: 1\nPaid :: 2\n\nCancel Order :: 0\nCancel Update :: -1";
    update = get_int(title, prompt, -1, 2);
    if(update == 1) status = "32";
    if(update == 2) status = "23";
  }

  else if(status == "31") {
    prompt += "Billed :: 1\n\nCancel Order :: 0\nCancel Update :: -1";
    update = get_int(title, prompt, -1, 1);
    if(update == 1) status = "32";
  }
  else if(status == "32") {
    prompt += "Paid :: 1\n\nCancel Order :: 0\nCancel Update :: -1";
    update = get_int(title, prompt, -1, 1);
    if(update == 1) status = "33";
  }
  else if(status == "13") {
    prompt += "Packaged :: 1\n\nCancel Order :: 0\nCancel Update :: -1";
    update = get_int(title, prompt, -1, 1);
    if(update == 1) status = "23";
  }
  else if(status == "23") {
    prompt += "Shipped :: 1\n\nCancel Order :: 0\nCancel Update :: -1";
    update = get_int(title, prompt, -1, 1);
    if(update == 1) status = "33";
  }
  if(update == 0) status = "44";
}
ofstream& operator<<(ofstream& ofs, const Order order) {
  ofs << 'o' + order.orderNumber + '|' + order.date;
  ofs << '|' + order.customer.get_customer_number();
  ofs << '|' + order.salesAssociate.get_employee_number();
  ofs << '|' + order.status;
  for(int i = 0; i < order.models.size(); i++) {
    ofs << "|!" + order.models[i].get_model_number();
  }
  return ofs;
}
//-----------------------------------------------------------------------S H O P
/** \class Shop
  *
  * \brief This is the central location where all records are kept and all objects are created.
  *
  * This class is meant to represent a Shop. Data can be added and retrieved to/from
  * the Shop via the create_new_object functions and get_object_list functions
  * respectively. */
class Shop {
  public:
    /** \brief Creates a new RobotPart.
      * \param choice of which RobotPart type to create.
      *
      * This method retrieves relevant information from the user about whichever part
      * they are creating via the get_int, get_double and get_string functions.
      * The user chooses which RobotPart to create via the GUI menubar. */
    void create_new_robot_part(int choice);
    ///Returns a string representation of a RobotPart from member robotParts.
    string part_to_string(int index);
    /** \brief Gets information about all RobotParts in Shop's records.
      * \return String representation of member robotParts. */
    string get_part_list();
    ///Returns string list of RobotParts of a given type based on input.
    string get_part_list(string type);

    /** \brief Create a new RobotModel.
      *
      * This method retrieves relevant infromation from the user about the a RobotModel,
      * then creates a new RobotModel and adds it to member robotModels. */
    void create_new_robot_model();
    ///Returns a string representation of a RobotModel from member robotModels.
    string model_to_string(int index);
    /** \brief Gets infromation about all RobotModels in Shop's records.
      * \return String representation of member robotModels. */
    string get_model_list();
    string get_basic_model_list();

    void create_new_customer();
    ///This returns a string representation of a Customer from member customers.
    string customer_to_string(int index);
    string get_customer_list();

    void create_new_sales_associate();
    ///This returns a string representation of a SalesAssociate from member salesAssociates.
    string associate_to_string(int index);
    string get_associate_list();

    void create_new_order();
    void manage_order();
    ///This returns a string representation of an Order from member orders.
    string order_to_string(int index);
    string get_order_list();
    string get_order_list_by_associate();

    void save(string filename);
    void open(string filename);
    string get_help();
    void easter_egg();
  private:
    int get_robot_part(string type);
    int get_customer();
    int get_sales_associate(string prompt);
    int get_robot_model();
    int get_order();

    vector<RobotPart*> robotParts; //!< Vector of RobotPart pointers. A list of all RobotParts created for this Shop.
    vector<RobotModel> robotModels; //!< Vector of RobotModels. A list of all RobotModels created for this Shop.
    vector<Customer> customers; //!< Vector of Customers. A list of all Customer accounts created for this Shop.
    vector<SalesAssociate> salesAssociates; //!< Vector of SalesAssociates. A list of all SalesAssociates that have worked for the Shop.
    vector<Order> orders; //!< Vector of Orders. A list of all Orders that have been created for this Shop.
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

  }
  else if(choice == 2) {
    display_message("Battery", "Gathering unique battery information...");
    double storedEnergy;

    type = "Battery";
    storedEnergy = get_double(name, "Max Energy[kWh]? ");
    part = new Battery{type, name, partNumber, cost, weight, description, imageFilename, storedEnergy};
    robotParts.push_back(part);

  }
  else if(choice == 3) {
    display_message("Head", "Gathering unique robot head information...");
    double power;

    type = "Head";
    power = get_double(name, "Power? ");
    part = new Head{type, name, partNumber, cost, weight, description, imageFilename, power};
    robotParts.push_back(part);

  }
  else if(choice == 4) {
    display_message("Locomotor", "Gathering unique locomotor information...");
    double power, maxSpeed;

    type = "Locomotor";
    power = get_double(name, "Max Power[W]? ");
    maxSpeed = get_double(name, "Max Speed[mph]? ");
    part = new Locomotor{type, name, partNumber, cost, weight, description, imageFilename, power, maxSpeed};
    robotParts.push_back(part);

  }
  else if(choice == 5) {
    display_message("Torso", "Gathering unique robot torso information...");
    int batteryCompartments, maxArms;

    type = "Torso";
    batteryCompartments = get_int(name, "Battery Compartments? ", 1, 3);
    maxArms = get_int(name, "Max Arms? ", 1, 2);
    part = new Torso{type, name, partNumber, cost, weight, description, imageFilename, batteryCompartments, maxArms};
    robotParts.push_back(part);

  }

  display_message("New "+type+" part", part_to_string(robotParts.size() - 1));
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

  display_message("New robot model", model_to_string(robotModels.size() - 1));
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

  display_message("New customer account", customer_to_string(customers.size() - 1));
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

  display_message("New sales associate account", associate_to_string(salesAssociates.size() - 1));
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
  vector<RobotModel> models;

  date = get_string("Order Creation", "Today's Date? ");
  orderNumber = get_string("Order Creation", "Order ID#? ");
  status = "00";

  index = get_customer();
  customer = customers[index];

  index = get_sales_associate("Which sales associate helped you?");
  salesAssociate = salesAssociates[index];

  string choice;
  while(true) {
    index = get_robot_model();
    models.push_back(robotModels[index]);

    choice = get_string("Order Creation","Would you like to add another robot model?[Y/N]");
    if((choice == "N") || (choice == "n")) break;
  }

  Order order = Order{orderNumber, date, customer, salesAssociate, models, status};
  orders.push_back(order);

  display_message("New order", order_to_string(orders.size() - 1));
}
void Shop::manage_order() {
  if(orders.size() == 0) {
    display_message("Error", "No orders on record");
    return;
  }

  int index = get_order();
  string status = orders[index].get_status();
  if((status == "44") || (status == "33")) {
    display_message("Error", "No updates can be made on this Order");
    return;
  }

  orders[index].update_status();
  display_message("Updated order", order_to_string(index));
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

  int index = get_sales_associate("Enter the name of the associate you would like to audit.");
  SalesAssociate associate = salesAssociates[index];
  string employeeNumber = associate.get_employee_number();

  string output = "Associate #"+employeeNumber+'\n';
  for(int i = 0; i < orders.size(); i++) {
    if(orders[i].get_sa_number() == employeeNumber) {
      output += order_to_string(index);
    }
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
  //Resolving data that is without dependencies
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
          arms_mn.push_back(atemp);
        }
        else if(c == '?') {
          getline(ss, btemp, '|');
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
      vector<RobotModel> models;
      //ID numbers for customer, salesassociate and robotmodel
      string customerNumber, employeeNumber, temp;
      vector<string> modelNumbers;
      int delcount = 0;
      char c;
      for(; ss.get(c);) {
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
          status += c;
        }
        else if(delcount > 4) break;
        cout << c;
      }
      cout << c << '\n';
      ss.putback(c);

      for(; ss.get(c);) {
        if(c == '!') {
          getline(ss, temp, '|');
          modelNumbers.push_back(temp);
        }
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
      for(int i = 0; i < modelNumbers.size(); i++) {
        for(int j = 0; j < robotModels.size(); j++) {
          if(robotModels[j].get_model_number() == modelNumbers[i]) {
            models.push_back(robotModels[j]);
            model_check = true;
          }
        }
      }
      //If data is correctly formed Order will be added.
      if(customer_check && employee_check && model_check) {
        Order order = Order{orderNumber, date, customer, salesAssociate, models, status};
        orders.push_back(order);
      }
    }
    else if(type = '~') break;

  }
}
string Shop::get_help() {
  string file_help = "File > Open - Load in saved data from user specified file.\n";
    file_help += "File > Save - Save Shop data to default file \"shop.txt\". Will create file if\n";
    file_help += "it does not exist.\nFile > Save As - Save Shop data to user specified file. Will create file if it\n";
    file_help += "does not exist.\nFile > Exit - Exits program safely.";
  string create_help = "Data is gathered via a series of prompts.\n\n";
    create_help += "Create > Order - Gathers data relevant to Order. Then adds order to Shop records.\n";
    create_help += "Create > Customer - Gathers data relevant to Customer account. Then adds\n";
    create_help += "account to Shop records.\n";
    create_help += "Create > Sales Associate - Gathers data relevant to Sales Associate account.\n";
    create_help += "Then adds account to Shop records.\n";
    create_help += "Create > Part > Arm - Gathers data relevant to Arm along with generic RobotPart\n";
    create_help += "data. Adds new Arm to Shop records.\n";
    create_help += "Create > Part > Battery - Gathers data relevant to Battery along with generic\n";
    create_help += "RobotPart data. Adds new Battery to Shop records.\n";
    create_help += "Create > Part > Head - Gathers data relevant to Head along with generic RobotPart\n";
    create_help += "data. Adds new Head to Shop records.\n";
    create_help += "Create > Part > Locomotor - Gathers data relevant to Locomotor along with generic\n";
    create_help += "RobotPart data. Adds new Locomotor to Shop records.\n";
    create_help += "Create > Part > Torso - Gathers data relevant to Torso along with generic\n";
    create_help += "RobotPart data. Adds new Torso to Shop records.\n";
    create_help += "Create > Robot Model - Gathers data relevant to Robot Model then adds it to\n";
    create_help += "Shop records.";
  string report_help = "All Shop record reports are presented inside of a dialog box.\n\n";
    report_help += "Report > Order > All Orders - Displays list of all orders in shop records.\n";
    report_help += "Report > Order > By Associate - Prompts user for a sales associate's name\n";
    report_help += "then lists all orders that were created by that associate.\n";
    report_help += "Report > Customer - Displays list of all customer accounts in shop records.\n";
    report_help += "Report > Associate - Displays list of all asociate accounts in shop records.\n";
    report_help += "Report > Part - Displays list of all parts in shop records.\n";
    report_help += "Report > Robot Model > Boss View - Displays list of all robot models in shop\n";
    report_help += "records.\n";
    report_help += "Report > Robot Model > Customer View - Displays a simplified list of all\n";
    report_help += "robot models in shop records.";
  string utility_help = "Utility > Help - Provides a dialog box with helpful information on all menu\n";
    utility_help += "options related to a given menu bar tab that is specified by the user.\n";
    utility_help += "Utility > Manage Order - Provides ability to change status of an order, only\n";
    utility_help += "relvant changes can be made, typically 3 status-changing options or less are\n";
    utility_help += "available at once.\n";
    utility_help += "Utility > Egg - Adds generic data to Shop records for help with testing.";

  string choice;
  while(true) {
    choice = get_string("Get Help", "Enter one of the menu bar titles to recieve help.\nAlternatively enter \"cancel\" to exit.");
    if((choice == "File") || (choice == "file"))
      return file_help;
    else if((choice == "Create") || (choice == "create"))
      return create_help;
    else if((choice == "Report") || (choice == "report"))
      return report_help;
    else if((choice == "Utility") || (choice == "utility"))
      return utility_help;
    else if((choice == "Choice") || (choice == "choice"))
      return "Canceled help request.";
    else
      display_message("Error", "Please try again.");
  }
}
void Shop::easter_egg() {
  display_message("Easter Egg", "Filling databases for testing...");
  string imageFilename = "image.png";

  vector<RobotPart*> arms;
  vector<RobotPart*> batteries;
  vector<RobotModel> models;

  RobotPart* arm = new Arm{"Arm", "ACME Arm", "5431", 49.95, 15, "Standard Issue", imageFilename, 8999};
  robotParts.push_back(arm);
  arms.push_back(arm);
  arms.push_back(arm);

  RobotPart* battery = new Battery{"Battery", "ACME Battery", "9627", 14.95, 0.5, "Standard Issue", imageFilename, 56000};
  robotParts.push_back(battery);
  batteries.push_back(battery);
  batteries.push_back(battery);

  RobotPart* head = new Head{"Head", "ACME Head", "1625", 149.95, 25, "Standard Issue", imageFilename, 8999};
  robotParts.push_back(head);

  RobotPart* locomotor = new Locomotor{"Locomotor", "ACME Locomotor", "1830", 249.95, 75.5, "Standard Issue", imageFilename, 8999, 500.5};
  robotParts.push_back(locomotor);

  RobotPart* torso = new Torso{"Torso", "ACME Torso", "1151", 99.95, 120, "Standard Issue", imageFilename, 2, 2};
  robotParts.push_back(torso);

  RobotModel model = RobotModel{"ACME Robo", "5843", torso, head, locomotor, arms, batteries, 1899.95};
  robotModels.push_back(model);
  models.push_back(model);

  Customer customer = Customer{"John Smith", "2448", "817-555-5555", "email@aol.com"};
  customers.push_back(customer);

  SalesAssociate associate = SalesAssociate{"David Williams", "4562"};
  salesAssociates.push_back(associate);

  Order order = Order{"999", "January 2, 1997", customer, associate, models, "00"};
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
  bool customerExists = false;
  int customerIndex;

  prompt = "Enter the name of the customer you wish to select.";

  display_message("Customer list", "Retrieving customer account information...");
  while(true) {
    display_message("Identify the relevant customer name", get_customer_list());

    customerName = get_string("Customer Account Selection", prompt);
    for(int i = 0; i < customers.size(); i++) {
      if(customers[i].get_name() == customerName) {
        customerExists = true;
        customerIndex = i;
      }
    }

    if(customerExists) return customerIndex;
    display_message("Error", "Please re-enter customer name.");
  }
}
int Shop::get_sales_associate(string prompt) {
  string associateName;
  bool associateExists;
  int associateIndex;

  display_message("Sales Associate list", "Retrieving sales associate information...");
  while(true) {
    display_message("Identify the relevant associate name", get_associate_list());

    associateName = get_string("Sales Associate Selection", prompt);
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

  prompt = "Enter the name of the robot model you wish to purchase.";

  display_message("Robot Model list", "Retrieving robot model information...");
  while(true) {
    display_message("Indentify the relevant model name", get_basic_model_list());

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
int Shop::get_order() {
  string orderNumber, prompt;
  bool orderExists;
  int orderIndex;

  prompt = "Enter the order number of the order you wish to manage.";

  display_message("Order list", "Retrieving order information...");
  while(true) {
    display_message("Identify the relevant order number", get_order_list());

    orderNumber = get_string("Order Selection", prompt);
    for(int i = 0; i < orders.size(); i++) {
      if(orders[i].get_order_number() == orderNumber) {
        orderExists = true;
        orderIndex = i;
      }
    }

    if(orderExists) break;
    display_message("Error", "Error - Please re-enter order number.");
  }

  return orderIndex;
}
// globel
Fl_Menu_Bar *menubar;
Shop shop;

//-------------------------------------------------------------C A L L B A C K S

//FILE
void openCB(Fl_Widget* w, void* p) {
  string filename = get_string("Open", "Enter a filename.");
  display_message("Open", "Importing data from "+filename);
  shop.open(filename);
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
  display_message("Order List", shop.get_order_list_by_associate());
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
  display_message("Help", "H", shop.get_help());
}
void manage_orderCB(Fl_Widget* w, void* p) {
  shop.manage_order();
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
      { "&Head", 0, (Fl_Callback *)new_headCB },
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
      { "&Boss View", 0, (Fl_Callback *)list_modelsCB },
      { "&Customer View", 0, (Fl_Callback *)list_models_basicCB },
      { 0 },
    { 0 },
  { "&Utility", 0, 0, 0, FL_SUBMENU },
    { "&Help", FL_ALT + 'h', (Fl_Callback *)helpCB },
    { "&Manage Orders", FL_ALT + 'm', (Fl_Callback *)manage_orderCB },
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
      { "&Head", 0, (Fl_Callback *)new_headCB },
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
    { "&Manage Orders", FL_ALT + 'm', (Fl_Callback *)manage_orderCB },
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

  const int width = 640;
	const int height = 480;
  Fl_Window *win = new Fl_Window{width, height, "Robo Boi"};

    menubar = new Fl_Menu_Bar(0, 0, width, 30);
    login();
    beacon.hide();

  win->show();
  return(Fl::run());
}
