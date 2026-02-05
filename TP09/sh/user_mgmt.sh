#!/bin/bash
set -euo pipefail

# Prompt for a unique group name
while true; do
  read -rp "Enter new group name: " groupname
  if getent group "$groupname" > /dev/null; then
    echo "Error: Group '$groupname' already exists. Try another name."
  elif [[ -z "$groupname" ]]; then
    echo "Error: Group name cannot be empty."
  else
    break
  fi
done

# Prompt for a unique username
while true; do
  read -rp "Enter new username: " username
  if getent passwd "$username" > /dev/null; then
    echo "Error: User '$username' already exists. Try another name."
  elif [[ -z "$username" ]]; then
    echo "Error: Username cannot be empty."
  else
    break
  fi
done

# Create group and user
sudo groupadd "$groupname"
sudo useradd -m -s /bin/bash -g "$groupname" "$username"

# Set password
sudo passwd "$username"

# Create directory at root with same name as user
sudo mkdir -p "/$username"

# Set ownership
sudo chown "$username":"$groupname" "/$username"

# Set permissions: full control for owner and group
sudo chmod 770 "/$username"

# Set sticky bit so only owner can delete
sudo chmod +t "/$username"

echo "User '$username' and group '$groupname' created successfully."
echo "Directory '/$username' created with ownership and permissions set."