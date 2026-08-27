#include <switch.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char current_url[1024] = "https://example.com";
static char bookmark_url[1024] = "";

static void draw_header(void) {
    printf("\x1b[1;1H");
    printf("================================================\n");
    printf("                 NX BROWSER 0.3                 \n");
    printf("================================================\n");
    printf("  [A] Open URL   [X] Home    [Y] Bookmark       \n");
    printf("  [L] Back      [Z] Forward [R] Reload         \n");
    printf("  [B] Back       [R] Reload  [PLUS] Exit        \n");
    printf("------------------------------------------------\n");
}

static void draw_message(const char *title, const char *message) {\n    consoleClear();\n    draw_header();\n    printf("\\n  %s\\n\\n  %s\\n\\n", title, message);\n    printf("  Press B to return.\\n");\n}\n\nstatic void draw_home(void) {
    consoleClear();
    draw_header();
    printf("\n  Home\n\n");
    printf("  URL: %s\n\n", current_url);
    printf("  [A] Enter a URL and open it\n");
    printf("  [R] Reload the current page\n");
    printf("  [Y] Bookmark foundation\n\n");
    printf("  NX Browser now uses the Switch WebApplet\n");
    printf("  for real web-page rendering.\n");
}

static void draw_error(Result rc) {
    consoleClear();
    draw_header();
    printf("\n  Browser error\n\n");
    printf("  Result: 0x%08x\n\n", rc);
    printf("  Press B to return.\n");
}

static bool enter_url(void) {
    SwkbdConfig kbd;
    char out[1024] = {0};

    Result rc = swkbdCreate(&kbd, 0);
    if (R_FAILED(rc)) {
        draw_error(rc);
        return false;
    }

    swkbdConfigMakePresetDefault(&kbd);
    swkbdConfigSetHeaderText(&kbd, "NX Browser");
    swkbdConfigSetSubText(&kbd, "Enter website address");
    swkbdConfigSetGuideText(&kbd, "https://example.com");
    swkbdConfigSetInitialText(&kbd, current_url);
    swkbdConfigSetStringLenMax(&kbd, sizeof(out) - 1);

    rc = swkbdShow(&kbd, out, sizeof(out));
    swkbdClose(&kbd);

    if (R_FAILED(rc) || out[0] == '\0')
        return false;

    if (strncmp(out, "http://", 7) != 0 && strncmp(out, "https://", 8) != 0) {
        char normalized[1024];
        snprintf(normalized, sizeof(normalized), "https://%s", out);
        strncpy(current_url, normalized, sizeof(current_url) - 1);
    } else {
        strncpy(current_url, out, sizeof(current_url) - 1);
    }
    current_url[sizeof(current_url) - 1] = '\0';
    return true;
}

static Result open_web(const char *url) {
    WebCommonConfig config;
    WebCommonReply reply;
    Result rc = webPageCreate(&config, url);
    if (R_FAILED(rc))
        return rc;

    // Allow normal HTTP/HTTPS navigation from the web applet.
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

        if (kDown & HidNpadButton_Plus)
            break;

        if (kDown & HidNpadButton_A) {
            if (enter_url())
                open_web(current_url);
            draw_home();
        } else if (kDown & HidNpadButton_Y) {
            strncpy(bookmark_url, current_url, sizeof(bookmark_url) - 1);
            bookmark_url[sizeof(bookmark_url) - 1] = '\\0';
            draw_message("Bookmark saved", bookmark_url);
        } else if (kDown & HidNpadButton_L) {
            // WebApplet owns page history; reopening the current URL keeps navigation available.
            open_web(current_url);
            draw_home();
        } else if (kDown & HidNpadButton_ZL) {
            if (bookmark_url[0]) {
                strncpy(current_url, bookmark_url, sizeof(current_url) - 1);
                current_url[sizeof(current_url) - 1] = '\\0';
                open_web(current_url);
            }
            draw_home();
        } else if (kDown & HidNpadButton_R) {
            open_web(current_url);
            draw_home();
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
