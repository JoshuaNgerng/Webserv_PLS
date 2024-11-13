// Function to fetch the file list from the server and populate the dropdown
function loadFileList() {
    // Send GET request to fetch the file list (in JSON format)
    fetch('/json/list/')
        .then(response => response.json())
        .then(data => {
            // Populate the file select dropdown with the file names
            const fileSelect = document.getElementById('file-select');
            fileSelect.innerHTML = ''; // Clear any existing options
            data.forEach(file => {
                const option = document.createElement('option');
                option.value = file.name;
                option.textContent = file.name;
                fileSelect.appendChild(option);
            });
        })
        .catch(error => {
            alert('Error fetching file list: ' + error);
        });
}

// Function to handle form submission and send DELETE request
function deleteFile() {
    const file = document.getElementById('file-select').value;
    if (!file) {
        alert('Please select a file');
        return;
    }

    // Send DELETE request to delete the selected file
    fetch('/cgi-bin/delete_file.pl', {
        method: 'DELETE',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: `file=${encodeURIComponent(file)}`
    })
    .then(response => {
        if (response.ok) {
            alert('File deleted successfully');
            loadFileList();  // Reload the file list after deletion
        } else {
            alert('Failed to delete file');
        }
    })
    .catch(error => {
        alert('Error: ' + error);
    });
}

// Load the file list when the page loads
window.onload = loadFileList;