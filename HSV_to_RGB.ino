int getRED(int h, int s, int v) {
  h = map(h, 0, 255, 0, 360);
  s = map(s, 0, 255, 0, 100);
  v = map(v, 0, 255, 0, 100);


  int c = v * s;
  int x = c * (1 - abs(((h / 60) % 2) - 1));
  int m = v*100 - c;

  float temp;

  if (h < 60) {
    temp = c;
  }
  else if (h < 120) {
    temp = x;
  }
  else if (h < 180) {
    temp = 0;
  }
  else if (h < 240) {
    temp = 0;
  }
  else if (h < 300) {
    temp = x;
  }
  else {
    temp = c;
  }
  return ((temp + m) * 255)/100000;


}
int getGREEN(int h, float s, float v) {
  h = map(h, 0, 255, 0, 360);
  s = map(s, 0, 255, 0, 100);
  v = map(v, 0, 255, 0, 100);
  float c = v * s;
  float x = c * (1 - abs(((h / 60) % 2) - 1));
  float m = v - c;

  float temp;

  if (h < 60) {
    temp = x;
  }
  else if (h < 120) {
    temp = c;
  }
  else if (h < 180) {
    temp = c;
  }
  else if (h < 240) {
    temp = x;
  }
  else if (h < 300) {
    temp = 0;
  }
  else {
    temp = 0;
  }
  return (temp + m) * 255;
}

int getBLUE(int h, float s, float v) {
  h = map(h, 0, 255, 0, 360);
  s = map(s, 0, 255, 0, 1);
  v = map(v, 0, 255, 0, 1);
  float c = v * s;
  float x = c * (1 - abs(((h / 60) % 2) - 1));
  float m = v - c;

  float temp;

  if (h < 60) {
    temp = 0;
  }
  else if (h < 120) {
    temp = 0;
  }
  else if (h < 180) {
    temp = x;
  }
  else if (h < 240) {
    temp = c;
  }
  else if (h < 300) {
    temp = c;
  }
  else {
    temp = x;
  }
  return (temp + m) * 255;
}
