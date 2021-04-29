#include<stdio.h>
#include<emscripten/html5.h>
#include<emscripten.h>

int myFunc(int type, const struct EmscriptenMouseEvent *eventInfo, void * userData)
{
    printf("fml\n");
    emscripten_run_script("document.getElementById('answer').innerHTML = 'Hello, emscripten world!'");
    return 0;
}

int main()
{
    emscripten_set_click_callback("#hello", NULL, 0, myFunc);
    return 0;
}