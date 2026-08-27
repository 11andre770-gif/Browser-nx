#include <switch.h>
#include <stdio.h>
#include <string.h>

static char current_url[1024] = "https://example.com";
static char bookmark_url[1024] = "";

static void draw_header(void) {
    printf("\033[1;1H");
    printf("================================================\n");
    printf("                 NX BROWSER 0.5                 \n");
    printf("================================================\n");
    printf("  [A] Open URL   [X] Home    [Y] Bookmark       \n");
    printf("  [L] Back       [ZL] Bookmark [R] Reload        \n");
    printf("  [PLUS] Exit\n");
    printf("------------------------------------------------\n");
}

static void draw_message(const char *title, const char *message) {
    consoleClear();
    draw_header();
    printf("\n  %s\n\n  %s\n\n", title, message);
    printf("  Press B to return.\n");
}

static void draw_home(void) {
    consoleClear();
    draw_header();
    printf("\n  Home\n\n");
    printf("  URL: %s\n\n", current_url);
    printf("  [A] Enter URL and open\n");
    printf("  [R] Open current URL\n");
    printf("  [Y] Save bookmark\n");
    if (bookmark_url[0] != '\0')
        printf("  [ZL] Open bookmark: %s\n", bookmark_url);
    printf("\n  NX Browser uses the Switch WebApplet\n");
    printf("  for real web-page rendering.\n");
}

static Result enter_url(void) {
    SwkbdConfig kbd;
    char out[1024] = {0};
    Result rc = swkbdCreate(&kbd, 0);
    if (R_FAILED(rc)) return rc;

    swkbdConfigMakePresetDefault(&kbd);
    swkbdConfigSetHeaderText(&kbd, "NX Browser");
    swkbdConfigSetSubText(&kbd, "Enter website address");
    swkbdConfigSetGuideText(&kbd, "https://example.com");
    swkbdConfigSetInitialText(&kbd, current_url);
    swkbdConfigSetStringLenMax(&kbd, sizeof(out) - 1);

    rc = swkbdShow(&kbd, out, sizeof(out));
    swkbdClose(&kbd);
    if (R_FAILED(rc) || out[0] == '\0') return rc;

    if (strncmp(out, "http://", 7) != 0 && strncmp(out, "https://", 8) != 0) {
        snprintf(current_url, sizeof(current_url), "https://%s", out);
    } else {
        strncpy(current_url, out, sizeof(current_url) - 1);
        current_url[sizeof(current_url) - 1] = '\0';
    }
    return 0;
}

static Result open_web(const char *url) {
    WebCommonConfig config;
    WebCommonReply reply;
    Result rc = webPageCreate(&config, url);
    if (R_FAILED(rc)) return rc;

    webConfigSetWhitelist(&config, "^https?://.*");
    webConfigSetPointer(&config, true);
    webConfigSetLeftStickMode(&config, WebLeftStickMode_Pointer);
    webConfigSetTouchEnabledOnContents(&config, true);
    webConfigSetJsExtension(&config, true);
    webConfigSetPageCache(&config, true);
    webConfigSetWebAudio(&config, true);
    webConfigSetScreenShot(&config, true);
    webConfigSetDisplayUrlKind(&config, true);
    webConfigSetFooter(&config, true);
    webConfigSetTransferMemory(&config, true);

    rc = webConfigShow(&config, &reply);
    if (R_SUCCEEDED(rc)) {
        char last_url[1024] = {0};
        size_t last_size = 0;
        if (R_SUCCEEDED(webReplyGetLastUrl(&reply, last_url, sizeof(last_url), &last_size)) &&
            last_url[0] != '\0') {
            strncpy(current_url, last_url, sizeof(current_url) - 1);
            current_url[sizeof(current_url) - 1] = '\0';
        }
    }
    return rc;
}

int main(int argc, char* argv[]) {
    consoleInit(NULL);
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    PadState pad;
    padInitializeDefault(&pad);
    draw_home();

    while (appletMainLoop()) {
        padUpdate(&pad);
        const u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) break;

        if (kDown & HidNpadButton_A) {
            Result rc = enter_url();
            if (R_SUCCEEDED(rc)) rc = open_web(current_url);
            if (R_FAILED(rc)) draw_message("Browser error", "Unable to open the web applet.");
            else draw_home();
        } else if (kDown & HidNpadButton_R) {
            Result rc = open_web(current_url);
            if (R_FAILED(rc)) draw_message("Browser error", "Unable to open the web applet.");
            else draw_home();
        } else if (kDown & HidNpadButton_Y) {
            strncpy(bookmark_url, current_url, sizeof(bookmark_url) - 1);
            bookmark_url[sizeof(bookmark_url) - 1] = '\0';
            draw_message("Bookmark saved", bookmark_url);
        } else if (kDown & HidNpadButton_ZL) {
            if (bookmark_url[0] != '\0') {
                strncpy(current_url, bookmark_url, sizeof(current_url) - 1);
                current_url[sizeof(current_url) - 1] = '\0';
                Result rc = open_web(current_url);
                if (R_FAILED(rc)) draw_message("Browser error", "Unable to open bookmark.");
            }
            draw_home();
        } else if (kDown & HidNpadButton_B) {
            draw_home();
        } else if (kDown & HidNpadButton_X) {
            draw_home();
        } else if (kDown & HidNpadButton_L) {
            draw_message("Navigation", "Page history is controlled by the system WebApplet.");
        }

        svcSleepThread(10000000ULL);
    }

    consoleExit(NULL);
    return 0;
}
