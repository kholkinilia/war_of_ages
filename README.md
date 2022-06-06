<h1 align="center">
  <br>
  <a href="https://github.com/kholkinilya/war_of_ages"><img src="https://github.com/kholkinilya/war_of_ages/blob/multiplayer_debug/client/resources/pictures/logo.png?raw=true" alt="War of ages" width="200"></a>
  <br>
  War of Ages
  <br>
</h1>

<h4 align="center"> A clone of well-known game <a href="https://igroutka.ru/flesh-igry/28216-age-of-war.html" target="_blank">Age of War</a>, extended with multiplayer.</h4>

<p align="center">
  <a href="https://github.com/kholkinilya/war_of_ages">
    <img src="https://img.shields.io/github/languages/top/kholkinilya/war_of_ages?logo=github&style=flat-square"
         alt="Github top language">
  </a>
  <a href="https://github.com/kholkinilya/war_of_ages/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/kholkinilya/war_of_ages?style=flat-square">
  </a>
  <a href="https://www.google.com/search?q=%D0%BA%D0%BE%D1%82%D1%8F%D1%82%D0%B0+%D0%B2+%D0%BA%D0%BE%D1%80%D0%B7%D0%B8%D0%BD%D0%BA%D0%B5&sxsrf=ALiCzsYMkSzTyIJ_FxOqX7Auo69DxZ0BPA:1654372913022&source=lnms&tbm=isch&sa=X&ved=2ahUKEwiq4rmuy5T4AhVLBxAIHUppCc8Q_AUoAXoECAEQAw&biw=1536&bih=714&dpr=1.25">
    <img src="https://img.shields.io/github/commit-activity/y/kholkinilya/war_of_ages">
  </a>
</p>

### Features

* Play a single player game just like in Age of War.
* Play a game with your friend or a random dude.
* Participate in tournaments.
* Make a bot play for you, so you can have a rest during the game (and, hopefully, not lose).
* See your stats, i.e. rating, number of games won/lost online, etc.
* Chat with your friends inplace during a tournament or a game.

### Demo video

Click on them:

[![Demo video](https://static8.depositphotos.com/1394326/864/i/600/depositphotos_8642715-stock-photo-six-in-a-basket.jpg)](https://www.youtube.com/watch?v=k8IB5wZuL_I&t=0s)

### Dependencies

* SFML
* TGUI
* boost::asio
* SQLite3

### Install on Windows

You will need `MSYS2`, which you can install [here](https://www.msys2.org/#installation).
It is supposed that you already have `cmake` and `git`. If not here is the 
[link](https://wiki.compscicenter.ru/index.php/C%2B%2B_1MIT_%D0%B2%D0%B5%D1%81%D0%BD%D0%B0_1_2022), where you can see how to install it.

Once you've installed it, run the following lines in `MSYS2` to install all libs except of `TGUI`:

```
pacman -S mingw-w64-x86_64-sfml 
pacman -S mingw-w64-x86_64-boost
pacman -S mingw-w64-x86_64-sqlite3 
```

Now open cmd and just run the following lines in your cmd
from an empty directory to install `TGUI` and to build the project:

``` 
git clone https://github.com/kholkinilya/war_of_ages.git
mkdir build
cd war_of_ages\build

git clone https://github.com/texus/TGUI.git
mkdir TGUI\build
cd TGUI\build
cmake .. -G="MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug -DTGUI_CXX_STANDARD=17 -DTGUI_BACKEND=SFML_GRAPHICS -DTGUI_BUILD_GUI_BUILDER=0
make -j4
cd ..\..

cmake .. -G="MSYS Makefiles" -DTGUI_DIR=.\TGUI\build
make -j4  

cp TGUI/build/lib/tgui-d.dll .
mkdir -p ../client/resources/sounds/music/

war_of_ages.exe
```

Last beat is to install music (it's optional actually).
Just put the music you like to play during the lobby in `client/resources/sounds/lobby_music.{wav, ogg, flac}`.
And also you can install a battle music into `client/resources/sounds/battle_music.{wav, ogg, flac}`.

