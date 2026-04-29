import tkinter as tk
import os
import subprocess
import time

# Always resolve paths relative to the project root (not the current working directory).
PROJECT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
DATA_DIR = os.path.join(PROJECT_DIR, "data")
INPUT_PATH = os.path.join(DATA_DIR, "input.txt")
OUTPUT_PATH = os.path.join(DATA_DIR, "output.txt")
EXE_PATH = os.path.join(PROJECT_DIR, "compiler")

# ---------- FILE FUNCTIONS ----------

def write_input(code):
    os.makedirs(DATA_DIR, exist_ok=True)
    with open(INPUT_PATH, "w", encoding="utf-8") as f:
        f.write(code)

def read_output():
    if not os.path.exists(OUTPUT_PATH):
        return "Output not generated yet."

    with open(OUTPUT_PATH, "r", encoding="utf-8") as f:
        return f.read()

def run_backend():
    if not os.path.exists(EXE_PATH):
        return (False, f"Backend not found: {EXE_PATH}\nBuild it first (gcc) then try again.")

    try:
        # Run from project root so the C program finds data/input.txt and data/output.txt.
        result = subprocess.run(
            [EXE_PATH],
            cwd=PROJECT_DIR,
            capture_output=True,
            text=True,
            timeout=10,
        )
        if result.returncode != 0:
            err = result.stderr.strip() or result.stdout.strip() or "Unknown error"
            return (False, f"Backend failed (exit {result.returncode}):\n{err}")
        return (True, "Backend ran successfully.")
    except Exception as e:
        return (False, f"Error running backend: {e}")

# ---------- BUTTON FUNCTIONS ----------

def send_code():
    code = code_input.get("1.0", "end-1c")

    write_input(code)

    ok, msg = run_backend()

    status_output.delete("1.0", "end")
    status_output.insert("end", f"{msg}\n")
    status_output.insert("end", f"Input: {INPUT_PATH}\nOutput: {OUTPUT_PATH}\n")
    status_output.insert("end", f"Time: {time.strftime('%H:%M:%S')}\n")

    if ok:
        show_output()

def show_output():
    output = read_output()

    output_box.delete("1.0", "end")
    output_box.insert("end", output)


# ---------- MAIN UI ----------

def run_app():
    global code_input, output_box, status_output

    root = tk.Tk()
    root.title("Mini Compiler Front-End")
    root.geometry("900x600")

    # Title
    tk.Label(root, text="Mini Compiler Front-End", font=("Arial", 16)).pack(pady=10)

    # Code Input
    tk.Label(root, text="Enter Source Code:").pack()
    code_input = tk.Text(root, height=10, width=100)
    code_input.pack()

    # Buttons
    button_frame = tk.Frame(root)
    button_frame.pack(pady=10)

    tk.Button(button_frame, text="Compile", command=send_code, width=15).pack(side=tk.LEFT, padx=10)
    tk.Button(button_frame, text="View Output", command=show_output, width=15).pack(side=tk.LEFT, padx=10)

    # Status Box
    tk.Label(root, text="Status:").pack()
    status_output = tk.Text(root, height=3, width=100)
    status_output.pack()

    # Output Box
    tk.Label(root, text="Compiler Output:").pack()
    output_box = tk.Text(root, height=15, width=100)
    output_box.pack()

    root.mainloop()

if __name__ == "__main__":
    run_app()
