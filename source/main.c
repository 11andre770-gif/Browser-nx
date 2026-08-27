#include <switch.h>
#include <stdio.h>
#include <string.h>

static void draw_header(void) {
    printf("\x1b[1;1H");
    printf("================================================\n");
    printf("                 NX BROWSER 0.2                 \n");
    printf("================================================\n");
    printf("  [A] Address    [X] Home    [Y] Bookmark       \n");
    printf("  [B] Back       [R] Reload  [PLUS] Exit        \n");
    printf("------------------------------------------------\n");
}

static void draw_home(void) {
    consoleClear();
    draw_header();
    printf("\n");
    printf("  Home\n\n");
    printf("  Address: https://example.com\n\n");
    printf("  Quick links:\n");
    printf("    [1] Example\n");
    printf("    [2] About this browser\n\n");
    printf("  Touch/controller web UI foundation ready.\n");
    printf("  Web engine integration is the next milestone.\n");
}

static void draw_message(const char *title, const char *message) {
    consoleClear();
    draw_header();
    printf("\n  %s\n\n  %s\n\n", title, message);
    printf("  Press B to return.\n");
}

int main(int argc, char* argv[]) {
    consoleInit(NULL);
    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    draw_home();

    while (appletMainLoop()) {
        padUpdate(&pad);
        const u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) break;

        if (kDown & HidNpadButton_A) {
            draw_message("Address", "Address entry / on-screen keyboard will be connected here.");
        } else if (kDown & HidNpadButton_Y) {
            draw_message("Bookmarks", "Bookmark manager foundation.");
        } else if (kDown & HidNpadButton_R) {
            draw_message("Reload", "Reload request prepared for the web engine.");
        } else if (kDown & HidNpadButton_B) {
            draw_home();
        } else if (kDown & HidNpadButton_X) {
            draw_home();
        }

        svcSleepThread(10000000ULL);
    }

    consoleExit(NULL);
    return 0;
}
