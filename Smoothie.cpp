/*
 * One of the kinds of fruits in a smoothie
 * All of the pineapples (or whatever) are grouped into one Smoothie_fruit
 * Also has a color which contributes to the final color of the smoothie
 */
class Smoothie_fruit {
  public:
    Smoothie_fruit(std::string name, double r,double g,double b);
    std::string name;
    int goal;
    int actual;
    double color[3];
};

Smoothie_fruit::Smoothie_fruit(std::string _name, double r,double g,double b) {
  name = _name;
  goal = (((double) rand()) / RAND_MAX) * 6;
  actual = 0;
  color[0] = r; color[1] = g; color[2] = b;
}

/*
 * Tracks the recipe and the actual fruits in the smoothie
 * also handles color of smoothie
 */
class Smoothie {
  public:
    Smoothie();
    std::vector <Smoothie_fruit*> fruits;
    void increment(const char*);
    void print();
    void calc_color();
    double color[4]; // recalc'd' when a fruit is added
    int contains_anything;
    int goals_achieved; // recalc'd' when a fruit is added
};

Smoothie::Smoothie() {
  srand(time(NULL)); // set rand seed to current time for more randomness
  contains_anything = 0;
  goals_achieved = 0;
  fruits.push_back(new Smoothie_fruit("pineapples", 1, 0.9, 0.3));
  fruits.push_back(new Smoothie_fruit("grapes", 0.5, 0.1, 0.6));
  fruits.push_back(new Smoothie_fruit("oranges", 0.9, 0.5, 0.2));
  fruits.push_back(new Smoothie_fruit("bananas", 1, 1, 0.1));
}

/*
 * Adds a fruit to the smoothie by name.
 */
void Smoothie::increment(const char* fruit_name) {
  for(unsigned int i = 0; i < fruits.size(); i++) {
    if(fruits[i]->name == fruit_name) {
      fruits[i]->actual++;
      break;
    }
  }
  calc_color();
}

void Smoothie::print() {
  std::string out = "Recipe:";
  for(unsigned int i = 0; i < fruits.size(); i++) {
    Smoothie_fruit* fruit = fruits[i];
    out += "\n  ";
    out += std::to_string(fruit->actual) + "/" + std::to_string(fruit->goal);
    out += " " + fruit->name;
  }
  PrintMultiline(out.c_str());
}

void Smoothie::calc_color() {
  double sums[] = {0, 0, 0};
  int count = 0;
  goals_achieved = 1;
  for(unsigned int i = 0; i < fruits.size(); i++) {
    Smoothie_fruit* fruit = fruits[i];
    for(unsigned int j = 0; j <= 3; j++) {
      sums[j] += fruit->color[j] * fruit->actual;
    }
    if(fruit->actual != fruit->goal) goals_achieved = 0;
    count += fruit->actual;
  }
  for(unsigned int i = 0; i <= 3; i++) {
    if(count == 0) {
      color[i] = 0.5;
    } else {
      color[i] = sums[i]/count;
    }
  }
  contains_anything = !!count;
}
