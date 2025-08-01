#include "include/stateManager.h"

void initStateManager(StateManager *stateMng) {
    *stateMng = calloc(1, sizeof(struct statemng));
    if (!*stateMng) {
        fprintf(stderr, "Failed to allocate memory for StateManager\n");
        exit(EXIT_FAILURE);
    }
}


void pushState(ZENg zEngine, GameState *state) {
    if (zEngine->stateMng->top < MAX_GAME_STATES) {
        GameState *curr = getCurrState(zEngine->stateMng);
        // call onExit if the pushed state is not an overlay
        if (curr && !state->isOverlay && curr->onExit) {
            curr->onExit(zEngine);
            printf("Called onExit for state %d\n", curr->type);
        }

        zEngine->stateMng->states[zEngine->stateMng->top++] = state;
        if (state->onEnter) {
            state->onEnter(zEngine);
            printf("Called onEnter for state %d\n", state->type);
        }

        printf("Exited state %d, entered state %d\n", curr ? curr->type : -1, state->type);
    }
}

void popState(ZENg zEngine) {
    if (zEngine->stateMng->top > 0) {
        // pop the top and call onExit
        GameState *currState = zEngine->stateMng->states[--zEngine->stateMng->top];
        if (currState->onExit) {
            currState->onExit(zEngine);
            printf("Called onExit for state %d\n", currState->type);
        }

        // and call onEnter for the new top if the popped one was not an overlay
        GameState *newState = getCurrState(zEngine->stateMng);
        if (newState && !currState->isOverlay && newState->onEnter) {
            newState->onEnter(zEngine);
            printf("Called onEnter for state %d\n", newState->type);
        }

        
        printf("Exited state %d, entered state %d\n", currState->type, newState->type);
        free(currState);
        currState = NULL;  // clear the pointer to avoid dangling pointer issues
    }
}

GameState* getCurrState(StateManager stateMng) {
    if (stateMng->top == 0) return NULL;
    return stateMng->states[stateMng->top - 1];
}
