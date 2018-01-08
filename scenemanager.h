#ifndef HEADER_SCENEMANAGER
#define HEADER_SCENEMANAGER

typedef enum {
  SCENE_TITLE = 1,
  SCENE_WAITOPPONENT,
  SCENE_MAINGAME
}Scene;

void InitSceneManager(int argc, char **argv);
void SceneManage();

extern Scene sceneStatus;

#endif