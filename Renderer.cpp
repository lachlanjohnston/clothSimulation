#include "Renderer.h"

Renderer::Renderer(Mesh* mesh_, GLFWwindow* window_) 
    : mesh(mesh_), window(window_) {};

void Renderer::initialize() {
    // initialize buffer states, shaders here
}

void Renderer::render() {
    // fetch vertices and update buffers
}

void Renderer::update() {
    // delegate required frame update tasks
}