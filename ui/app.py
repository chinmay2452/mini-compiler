import tkinter as tk
import os

# ---------- FILE FUNCTIONS ----------

def write_input(code):
    with open("data/input.txt", "w") as f:
        f.write(code)

def read_output():
    if not os.path.exists("data/output.txt"):
        return "Output not generated yet."

    with open("data/output.txt", "r") as f:
        return f.read()


# ---------- BUTTON FUNCTIONS ----------

def send_code():
    code = code_input.get("1.0", "end")

    write_input(code)

    status_output.delete("1.0", "end")
    status_output.insert("end", "✅ Code sent to compiler.\nNow run C backend.")

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