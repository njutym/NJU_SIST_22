import win32api
import win32process
import win32con
import win32security
import win32com.client
import sys
import ctypes


def inject_dll_into_process(process_id, dll_path):
    process_handle = win32api.OpenProcess(
        win32con.PROCESS_ALL_ACCESS, False, process_id)
    if process_handle:
        try:
            # Allocate memory in the target process to store the path to the DLL
            path_addr = win32api.VirtualAllocEx(process_handle, 0, len(
                dll_path), win32con.MEM_COMMIT, win32con.PAGE_READWRITE)
            if path_addr:
                # Write the path to the DLL into the target process
                win32api.WriteProcessMemory(
                    process_handle, path_addr, dll_path.encode('utf-8'))
                # Get the address of LoadLibraryA function from kernel32
                winmm = win32api.GetModuleHandle("winmm.dll")
                load_library_addr = win32api.GetProcAddress(
                    winmm, "timegettime")
                if load_library_addr:
                    # Create a remote thread to load the DLL into the target process
                    remote_thread = win32api.CreateRemoteThread(
                        process_handle, None, 0, load_library_addr, path_addr, 0, None)
                    if remote_thread:
                        win32api.WaitForSingleObject(
                            remote_thread, win32con.INFINITE)
                        win32api.CloseHandle(remote_thread)
        finally:
            win32api.CloseHandle(process_handle)


def hook_timeGetTime(dll_path):
    # Inject the DLL into the target process
    target_process_id = 1234  # Replace with the target process ID
    inject_dll_into_process(target_process_id, dll_path)


# Replace 'your_hook.dll' with the path to your DLL
if len(sys.argv) > 1 and sys.argv[1] == "hook":
    hook_timeGetTime("C:\\path\\to\\your_hook.dll")
    print("Hooked timeGetTime in the target process.")
else:
    print("Run this script with 'hook' as an argument to inject the DLL and hook timeGetTime.")
