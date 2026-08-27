#include <switch.h>
#include <stdio.h>
#include <string.h>

static void draw_ui(void) {
    consoleClear();
    printf("\x1b[1;1H");
    printf("NX Browser 0.1\n");
    printf("------------------------------\n");
    printf("Standalone Switch browser shell\n\n");
    printf("[A] Open address\n");
    printf("[X] Home\n");
    printf("[Y] Bookmark\n");
    printf("[B] Back\n");
    printf("[PLUS] Exit\n\n");
    printf("Address: https://example.com\n\n");
    printf("Next milestone:\n");
    printf("  Web engine + HTTPS + touch UI\n");
}

int main(int argc, char* argv[]) {
    consoleInit(NULL);
    draw_ui();

    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    while (appletMainLoop()) {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) break;

        if (kDown & HidNpadButton_A) {
            consoleClear();
            printf("NX Browser\n\n");
            printf("Address entry will be connected to the\n");
            printf("on-screen keyboard in the next milestone.\n\n");
            printf("Press B to return.\n");
        }

        if (kDown & HidNpadButton_B) draw_ui();

        if (kDown & HidNpadButton_X) draw_ui();

        svcSleepThread(10000000ULL);
    }

    consoleExit(NULL);
    return 0;
}
