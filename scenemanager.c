#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"scenemanager.h"
#include"findopponent.h"
#include"gamecontroller.h"
#include"ocelostone.h"

Scene sceneStatus;
char *serverName;
char *serverService;

void InitSceneManager(int argc, char **argv) {
  sceneStatus = SCENE_WAITOPPONENT;
  serverName = argv[1];
  serverService = argv[2];
}

void SceneManage() {
  switch(sceneStatus) {
    case SCENE_MAINGAME:
      ControlGameWithState();
      break;
    case SCENE_WAITOPPONENT:
      FindOpponentScene(serverName, serverService);
      break;
    case SCENE_TITLE:
      break;
  }
}