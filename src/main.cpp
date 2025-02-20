#include <app.h>

int main(int argc, char **argv) {
    App app = App(600, 600);
    bool initialized = false;
    initialized = app.init();
    if (initialized) {
        while(app.run()){}
        app.close();
    }
}