#!/bin/bash

# Initialize address book file if it doesn't exist
if [ ! -f address.txt ]; then
    touch address.txt
fi

while true; do
    # Display menu
    echo "Address Book Management System"
    echo "1. Search Address Book"
    echo "2. Add an address book entry"
    echo "3. Remove an address book entry"
    echo "4. Quit the program"
    echo -n "Enter your choice (1-4): "
    read choice

    case $choice in
        1)  # Search
            echo -n "Enter search term (ID/Name/Phone): "
            read search_term
            echo "Search Results:"
            echo "ID;Name;Phone"
            
            result=$(grep -i "$search_term" address.txt)

            if [ -n "$result" ]; then
                echo "$result"
            else
                echo "No Results Found"
            fi
            ;;
            
        2)  # Add entry
            echo -n "Enter ID: "
            read id
            echo -n "Enter Name: "
            read name
            echo -n "Enter Phone: "
            read phone
            
            # Check if ID already exists
            if grep -q "^$id;" address.txt; then
                echo "Error: ID already exists!"
            else
                echo "$id;$name;$phone" >> address.txt
                echo "Entry added successfully!"
            fi
            ;;
            
        3)  # Remove entry
            echo -n "Enter ID to remove: "
            read remove_id
            
            # Check if ID exists
            if grep -q "^$remove_id;" address.txt; then
                sed -i "/^$remove_id;/d" address.txt
                echo "Entry removed successfully!"
            else
                echo "Error: ID not found!"
            fi
            ;;
            
        4)  # Quit
            echo "Thank you for using Address Book Management System"
            exit 0
            ;;
            
        *)  # Invalid choice
            echo "Invalid choice! Please enter a number between 1 and 4."
            ;;
    esac
    
    echo -e "\nPress Enter to continue..."
    read
    clear
done