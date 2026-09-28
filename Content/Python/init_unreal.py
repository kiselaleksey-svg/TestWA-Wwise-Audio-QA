import unreal
from pathlib import Path

COMMAND_FILE = Path(r"L:\UETest\ue_test_command.txt")
STATUS_FILE = Path(r"L:\UETest\ue_test_status.txt")

level_editor = unreal.get_editor_subsystem(
    unreal.LevelEditorSubsystem
)

last_command = None


def write_status(status):
    STATUS_FILE.write_text(status, encoding="utf-8")
    unreal.log(f"[CI] STATUS: {status}")


def on_tick(delta_time):
    global last_command

    if not COMMAND_FILE.exists():
        return

    try:
        command = COMMAND_FILE.read_text(
            encoding="utf-8"
        ).strip().upper()
    except Exception as e:
        unreal.log_error(f"[CI] Cannot read command: {e}")
        return

    if not command or command == last_command:
        return

    last_command = command

    if command == "START":
        if level_editor.is_in_play_in_editor():
            write_status("PIE_ALREADY_RUNNING")
            return

        write_status("PIE_START_REQUESTED")
        level_editor.editor_request_begin_play()

    elif command == "STOP":
        if not level_editor.is_in_play_in_editor():
            write_status("PIE_ALREADY_STOPPED")
            return

        write_status("PIE_STOP_REQUESTED")
        level_editor.editor_request_end_play()

    elif command == "STATUS":
        if level_editor.is_in_play_in_editor():
            write_status("PIE_RUNNING")
        else:
            write_status("PIE_STOPPED")


def monitor_pie_state(delta_time):
    if level_editor.is_in_play_in_editor():
        current = "PIE_RUNNING"
    else:
        current = "PIE_STOPPED"

    try:
        previous = STATUS_FILE.read_text(
            encoding="utf-8"
        ).strip()
    except Exception:
        previous = ""

    if previous in {
        "PIE_START_REQUESTED",
        "PIE_STOP_REQUESTED",
    }:
        write_status(current)


unreal.register_slate_post_tick_callback(on_tick)
unreal.register_slate_post_tick_callback(monitor_pie_state)

write_status("CONTROLLER_READY")
unreal.log("[CI] Unreal Jenkins controller loaded.")