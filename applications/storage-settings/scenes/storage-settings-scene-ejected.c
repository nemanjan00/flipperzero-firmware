#include "../storage-settings.h"

static void
    storage_settings_scene_unmounted_dialog_callback(DialogExResult result, void* context) {
    StorageSettings* app = context;

    view_dispatcher_send_custom_event(app->view_dispatcher, result);
}

void storage_settings_scene_unmounted_on_enter(void* context) {
    StorageSettings* app = context;
    FS_Error error = storage_sd_unmount(app->fs_api);
    DialogEx* dialog_ex = app->dialog_ex;

    dialog_ex_set_left_button_text(dialog_ex, "Back");

    if(error == FSE_OK) {
        dialog_ex_set_header(dialog_ex, "SD card unmounted", 64, 10, AlignCenter, AlignCenter);
        dialog_ex_set_text(
            dialog_ex, "Now the SD card\ncan be removed.", 64, 32, AlignCenter, AlignCenter);
        notification_message(app->notification, &sequence_blink_green_100);
    } else {
        dialog_ex_set_header(
            dialog_ex, "Cannot unmount SD Card", 64, 10, AlignCenter, AlignCenter);
        dialog_ex_set_text(
            dialog_ex, storage_error_get_desc(error), 64, 32, AlignCenter, AlignCenter);
        notification_message(app->notification, &sequence_blink_red_100);
    }

    dialog_ex_set_context(dialog_ex, app);
    dialog_ex_set_result_callback(dialog_ex, storage_settings_scene_unmounted_dialog_callback);

    view_dispatcher_switch_to_view(app->view_dispatcher, StorageSettingsViewDialogEx);
}

bool storage_settings_scene_unmounted_on_event(void* context, SceneManagerEvent event) {
    StorageSettings* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case DialogExResultLeft:
            consumed = scene_manager_search_and_switch_to_previous_scene(
                app->scene_manager, StorageSettingsStart);
            break;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        consumed = scene_manager_search_and_switch_to_previous_scene(
            app->scene_manager, StorageSettingsStart);
    }

    return consumed;
}

void storage_settings_scene_unmounted_on_exit(void* context) {
    StorageSettings* app = context;
    DialogEx* dialog_ex = app->dialog_ex;

    dialog_ex_set_header(dialog_ex, NULL, 0, 0, AlignCenter, AlignCenter);
    dialog_ex_set_text(dialog_ex, NULL, 0, 0, AlignCenter, AlignTop);
    dialog_ex_set_icon(dialog_ex, 0, 0, NULL);
    dialog_ex_set_left_button_text(dialog_ex, NULL);
    dialog_ex_set_right_button_text(dialog_ex, NULL);
    dialog_ex_set_result_callback(dialog_ex, NULL);
    dialog_ex_set_context(dialog_ex, NULL);
}
