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

* [SFML](https://www.sfml-dev.org/)
* [TGUI](https://tgui.eu/)
* [boost::asio](https://www.boost.org/doc/libs/1_75_0/doc/html/boost_asio.html)
* [SQLite3](https://www.sqlite.org/index.html)

### Install on Windows

Sorry, we didn't manage to make it easy to install all dependencies with CMake, so 
you will need to ~~suffer~~ try hard to install the project.
To be clear, we don't guarantee that the method below works. 

For example, we didn't manage to install War of Ages on a random computer run on Windows in 1.5 hours. `:(`
Whereas, on another computer everything went smoothly and the project was installed in 7 minutes.

So, good luck!

You will need `MSYS2`, which you can install [here](https://www.msys2.org/#installation).
You will also need [CMake](https://cmake.org/) and [git](https://git-scm.com/).

To install `CMake` you could use `MSYS2` also:

```shell
pacman -S mingw-w64-x86_64-cmake 
pacman -S make
```

Also, make sure you've added directories with `cmake.exe` and `make.exe` to your `PATH` variable.

Once you've installed them, run the following lines in `MSYS2` to install all libs except of `TGUI`:

```shell
pacman -S mingw-w64-x86_64-sfml 
pacman -S mingw-w64-x86_64-boost
pacman -S mingw-w64-x86_64-sqlite3 
```

If you are getting error while installing packages from `MSYS2` try updating packages info with:

```shell
pacman -Syyu
```

Now open cmd and just run the following code line by line (skipping comments obviously) from an empty
directory to install `TGUI` and build the project:

```shell
git clone https://github.com/kholkinilya/war_of_ages.git
mkdir -p war_of_ages\build
cd war_of_ages\build

git clone https://github.com/texus/TGUI.git
mkdir -p TGUI\build
cd TGUI\build

# MSYS Makefiles is one of possible generators, you can use another one
cmake .. -G="MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug -DTGUI_CXX_STANDARD=17 -DTGUI_BACKEND=SFML_GRAPHICS -DTGUI_BUILD_GUI_BUILDER=0 
cmake --build . -j4
cd ..\..

cmake .. -G="MSYS Makefiles" -DTGUI_DIR=.\build\TGUI\build
cmake --build . -j4

cp TGUI\build\lib\tgui-d.dll .\client
mkdir -p ..\client\resources\sounds\music

client\war_of_ages_client.exe
```

Last beat is to install music (it's optional actually).
* Just put the music you'd like to play while in lobby in `client/resources/sounds/lobby_music.{wav, ogg, flac}`.

* Also, you can install a battle music into `client/resources/sounds/battle_music.{wav, ogg, flac}`.

We personally recommend [this one](https://www.chosic.com/download-audio/29678/) for lobby and [this one](https://opengameart.org/content/campaign-more-music-inside?destination=node/25538) (`Middle_age_RPG_Theme_1.ogg`) for battle.

### Install on Linux

Seems to us that you are capable of installing it yourself since you are a Linux user.
