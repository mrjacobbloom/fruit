# Fruit

This was my final project for a class on OpenGL graphics. It's a game in which
the user creates a smoothie by dragging fruits into a blender.

Click and drag fruits into the blender following the recipe in the top-left
corner of the screen. When you're done, click the blender to blend up your
smoothie. Text will appear letting you know whether you've won.

## Inspiration

This game was inspired by a flashback I had to the kids' MMO Webkinz, in which
there was a minigame where you'd do something similar.

## Usage

To run:
```shell
make clean; make; ./final
```

Note: I've never tried compiling on Windows so who knows if that'll work.

### Keybindings
  - `ESC` - exit.
  - `m` - toggle between static and mobile camera mode
  - arrow keys - move camera if in mobile camera mode

## Issues

- The dragging code is completely cheat-y and won't work if you resize the
  window or go into mobile camera mode

## Credits

A lot of this code is adapted from example code by my instructor, Willem
"Vlakkies" Schreuder. The original code is available
[here](http://www.prinmath.com/csci5229/Su18/programs/index.html).

### Textures

Most of the textures in this program are heavily modified from the source
images.

- Kitchen
  - [granite (tile)](https://www.deviantart.com/koncaliev/art/hi-res-seamless-granite-tiles-texture-343772373)
  - [granite (countertop)](http://texturelib.com/texture/?path=/Textures/rock/stones/rock_stones_0104)
  - [cabinets](http://www.partum.me/kitchen-cabinet-door-handles-amazon/kitchen-cabinet-door-handles-amazon-architecture-cabinet-hardware-at-the-home-depot-popular-kitchen-cabinets-handles-2-decorating-from-kitchen-kitchen-cupboard-door-handles-amazon/)
- Fruits
  - [leaves](https://blenderartists.org/t/realistic-grass-attempt/618665/18)
  - [pineapple](https://www.redbubble.com/people/expix/works/22374244-pineapple-texture)
  - [orange](https://pixabay.com/en/citrus-fruit-skin-orange-texture-2523487/)
  - [banana](https://texturefabrik.com/2016/04/20/7-banana-textures/)
- Other
  - [smoothie](https://www.marthastewart.com/1515914/savory-parsley-smoothie)
