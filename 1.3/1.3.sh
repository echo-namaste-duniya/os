#!/bin/bash

# Initialize address book file if it doesn't exist
ADDRESSBOOK="addressbook.txt"
if [ ! -f "$ADDRESSBOOK" ]; then
    touch "$ADDRESSBOOK"
fi

# Function to display records
display_records() {
    local search_term="$1"
    echo "ID:Name:Address:Phone"
    echo "------------------------"
    if [ -z "$search_term" ]; then
        cat "$ADDRESSBOOK"
    else
        grep -i "$search_term" "$ADDRESSBOOK"
    fi
}

# Function to search address book
search_addressbook() {
    echo -n "Enter search term (ID/Name/Address/Phone): "
    read search_term
    if [ -z "$search_term" ]; then
        echo "Search term cannot be empty!"
        return
    fi
    echo "Search Results:"
    display_records "$search_term"
}

# Function to add entries
add_entry() {
    echo -n "Enter ID: "
    read id
    
    # Check if ID already exists
    if grep -q "^$id:" "$ADDRESSBOOK"; then
        echo "Error: ID already exists!"
        return
    fi
    
    echo -n "Enter Name: "
    read name
    echo -n "Enter Address: "
    read address
    echo -n "Enter Phone: "
    read phone
    
    echo "$id:$name:$address:$phone" >> "$ADDRESSBOOK"
    echo "Entry added successfully!"
    echo "Added record:"
    grep "^$id:" "$ADDRESSBOOK"
}

# Function to edit an entry
edit_entry() {
    echo -n "Enter name to edit: "
    read name

    if grep -iq "$name" addressbook.txt; then
        grep -i "$name" addressbook.txt
    
        # Remove old record
        sed -i "/$name:/d" addressbook.txt

        # Get new details
        echo -n "Enter new ID: "
        read id
        echo -n "Enter new Name: "
        read new_name
        echo -n "Enter new Address: "
        read address
        echo -n "Enter new Phone: "
        read phone

        # Add updated record
        echo "$id:$new_name:$address:$phone" >> addressbook.txt
        echo " Entry updated successfully!"
    else
        echo " No record found with that name."
    fi
}


# Function to remove/edit entries
remove_edit_entry() {
    echo "1. Remove entry"
    echo "2. Edit entry"
    echo -n "Enter your choice (1-2): "
    read choice
    
    echo -n "Enter ID of the entry: "
    read id
    
    if ! grep -q "^$id:" "$ADDRESSBOOK"; then
        echo "Error: ID not found!"
        return
    fi
    
    echo "Current record:"
    grep "^$id:" "$ADDRESSBOOK"
    
    case $choice in
        1)  # Remove
            echo -n "Are you sure you want to delete this entry? (y/n): "
            read confirm
            if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
                sed -i "/^$id:/d" "$ADDRESSBOOK"
                echo "Entry removed successfully!"
            else
                echo "Deletion cancelled."
            fi
            ;;
        2)  # Edit
            edit_entry
            ;;
        *)
            echo "Invalid choice!"
            ;;
    esac
}

# Main menu loop
while true; do
    echo -e "\nAddress Book Management System"
    echo "1. Search address book"
    echo "2. Add entries"
    echo "3. Remove / edit entries"
    echo "4. Display all entries"
    echo "5. Quit"
    echo -n "Enter your choice (1-5): "
    read choice

    case $choice in
        1) search_addressbook ;;
        2) add_entry ;;
        3) remove_edit_entry ;;
        4) display_records ;;
        5) echo "Thank you for using Address Book Management System"
           exit 0 ;;
        *) echo "Invalid choice! Please enter a number between 1 and 5." ;;
    esac
    
    echo -e "\nPress Enter to continue..."
    read
    clear
done