class ButtonResource {
public:
  int id;
  bool state;

  ButtonResource(int id, bool state) {
    this->id = id;
    this->state = state;
  }
};