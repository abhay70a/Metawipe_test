import mysql.connector
import hashlib
import datetime

# ✅ Connect to MySQL
def connect_db():
    return mysql.connector.connect(
        host="localhost",
        user="root",
        password="root@123",
        database="project_db"   # ✅ make sure this says 'project_db'
    )

# ✅ Hash function (optional but secure)
def hash_password(password):
    return hashlib.sha256(password.encode()).hexdigest()

# ✅ Login function
def login():
    username = input("Enter username: ")
    password = input("Enter password: ")

    db = connect_db()
    cursor = db.cursor(dictionary=True)

    cursor.execute("SELECT * FROM users WHERE username=%s", (username,))
    user = cursor.fetchone()

    if not user:
        print("❌ User not found.")
        return None

    if user['password'] != password:
        print("❌ Invalid password.")
        return None

    print(f"✅ Welcome, {username} (Role ID: {user['role_id']})")
    return user

# ✅ Fetch permissions
def get_permissions(role_id):
    db = connect_db()
    cursor = db.cursor(dictionary=True)

    query = """
        SELECT p.permission_name
        FROM permissions p
        JOIN role_permissions rp ON p.permission_id = rp.permission_id
        WHERE rp.role_id = %s
    """
    cursor.execute(query, (role_id,))
    perms = [row['permission_name'] for row in cursor.fetchall()]
    return perms

# ✅ Log user actions
def log_action(user_id, action):
    db = connect_db()
    cursor = db.cursor()
    cursor.execute("INSERT INTO logs (user_id, action) VALUES (%s, %s)", (user_id, action))
    db.commit()

# ✅ Dummy OS-level functions (you’ll connect with your wipe.py later)
from core.wipe import secure_wipe  # ✅ Add this import at the top
def perform_admin_task(user_id):
    filepath = input("Enter the path of the file to securely delete: ")
    secure_wipe(filepath)  # 🧹 Calls the OS-level wipe function
    log_action(user_id, f"Performed secure wipe on {filepath}")

def perform_user_task(user_id):
    print("📄 Viewing system logs...")
    log_action(user_id, "Viewed system logs")

# ✅ Main CLI
def main():
    user = login()
    if not user:
        return

    permissions = get_permissions(user['role_id'])
    print("Your Permissions:", permissions)

    # Role-based control
    while True:
        print("\nChoose an action:")
        print("1. Perform admin action" if "admin_access" in permissions else "", end=" ")
        print("2. View logs" if "view_logs" in permissions else "", end=" ")
        print("3. Logout")

        choice = input("\nEnter choice: ")

        if choice == "1" and "admin_access" in permissions:
            perform_admin_task(user['user_id'])
        elif choice == "2" and "view_logs" in permissions:
            perform_user_task(user['user_id'])
        elif choice == "3":
            print("👋 Logged out.")
            break
        else:
            print("❌ Invalid choice or permission denied.")

if __name__ == "__main__":
    main()