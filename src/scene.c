#include "scene.h"

Scene *current_scene = NULL;

void scene_load(Scene *scene)
{
    if (current_scene && current_scene->quit) {
        current_scene->quit();
    }

    current_scene = scene;
    current_scene->init();
}
