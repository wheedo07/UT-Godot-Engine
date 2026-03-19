#include "mainNode.h"
#include "env.h"

MainNode::MainNode() {
    isClear = false;
    loop_count = 0;
    next_sequence_id = 0;
}

MainNode::~MainNode() {}

void MainNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("clear_system", "is"), &MainNode::clear_system, DEFVAL(true));
}

void MainNode::_ready() {
    SceneContainer::_ready();
    loader = ResourceLoader::get_singleton();
    sys = this;
}

void MainNode::_process(double delta) {
    system(delta);
}

Ref<Dialogues> MainNode::dia() {
    Ref<Dialogues> dialogue = memnew(Dialogues);
    return dialogue;
}