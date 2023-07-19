#include "Image.h"
#include "StickerSheet.h"

int main() {
  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //
  Image* saul = new Image();
  saul->readFromFile("../bettercallsaul.png");
  
  Image* johnWick = new Image();
  johnWick->readFromFile("../johnwick.png");
  johnWick->scale(2);

  Image* goku = new Image();
  goku->readFromFile("../gokudrip.png");
  goku->scale(0.5);

  Image foellinger;
  foellinger.readFromFile("../foellinger.png");

  StickerSheet myImage(foellinger, 3);
  
  myImage.addSticker(*saul, 400, 1500);
  myImage.addSticker(*johnWick, 750, 1500);
  myImage.addSticker(*goku, 550, 1500);

  Image completeThing(myImage.render());

  completeThing.scale(0.5);
  completeThing.writeToFile("../myImage.png");

  delete goku;
  delete johnWick;
  delete saul;
  return 0;
}
