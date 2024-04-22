// Framework CRUDS js

let tableName = null;
let actualFieldNames = null;
let fieldNames = null;

async function initializeApplication() {
    const metaData = await getMetaData(['TableName.cfg', 'FieldNames.cfg']);
    tableName = metaData[0]['data'];
    actualFieldNames = metaData[1]['data'];
    createElements();
    await showAllRecords();
}

async function getMetaData(pFileNames) {
    try {
        const metaData = await Promise.all(pFileNames.map(async (filename) => {
            const response = await fetch(filename);
            const data = await response.text();
            return { filename, data: filename === 'FieldNames.cfg' ? data.split('\n').map(name => name.trim()) : data.trim() };
        }));
        console.log(metaData[0], metaData[1]);
        return metaData;
    } catch (error) {
        console.error('Error fetching file:', error);
        return null;
    }
}

function createElements() {
    var counter = 0;
    var inputContainer = document.createElement('div');
    inputContainer.id = 'InputContainer';
    document.body.appendChild(inputContainer);
    for (counter = 0; counter < actualFieldNames.length; counter++) {
        var textBoxLabel = document.createElement('label');
        textBoxLabel.textContent = 'Enter ' + actualFieldNames[counter] + ': ';

        var textBox = document.createElement('input');
        textBox.setAttribute('type', 'text');
        textBox.setAttribute('name', actualFieldNames[counter]);
        textBox.setAttribute('id', actualFieldNames[counter]);
        textBox.setAttribute('PlaceHolder', actualFieldNames[counter]);

        inputContainer.appendChild(textBoxLabel);
        inputContainer.appendChild(textBox);
    }
    const buttons = ['Save', 'Clear'];
    const buttonFunctions = [saveRecord, resetInputfieldNames];
    for (counter = 0; counter < buttons.length; counter++) {
        var button = document.createElement('button');
        button.textContent = buttons[counter];
        button.addEventListener('click', buttonFunctions[counter]);
        inputContainer.appendChild(button);
    }
    
    var TableContainer = document.createElement('div');
    TableContainer.id = 'TableContainer';
    document.body.appendChild(TableContainer);
}

function getRecordDetails() {
    const fieldValues = [];
    for (var columnCounter = 0; columnCounter < actualFieldNames.length; columnCounter++) {
        fieldValues.push(encodeURIComponent(document.getElementById(actualFieldNames[columnCounter]).value));
    }
    return fieldValues;
}

function reorderJson(jsonData) {
    fieldNames = actualFieldNames.concat(['Update', 'Delete']);
    return jsonData.map(record => {
        const reorderedRecord = {};
        fieldNames.forEach(fieldName => {
            reorderedRecord[fieldName] = record[fieldName];
        });
        return reorderedRecord;
    });
}

async function saveRecord() {
    const recordDetails = getRecordDetails();
    try {
        const insertUrl = 'InsertRecord.jsp?TableName=' + tableName + '&FieldNames=' + actualFieldNames + '&FieldValues=' + recordDetails;
        console.log(insertUrl);
        const response = await fetch(insertUrl);
        const data = await response.text();
        if (data.trim() === "1") {
            console.log(tableName + ' with ' + actualFieldNames[0] + ' - ' + recordDetails[0] + ' inserted successfully.');
            const recordId = recordDetails[0];
            console.log(recordId);
            if (document.querySelector('button').textContent === 'Update') {
                deleteRecordLocally(recordId);
            }
            const tableRow = await getNewRecord(recordId);
            console.log(tableRow);
            const newRow = document.createElement('tr');
            fieldNames.forEach(fieldName => {
                const newCell = document.createElement('td');
                console.log(tableRow[0][fieldName]);
                newCell.innerHTML = tableRow[0][fieldName];
                newRow.appendChild(newCell);
            });
            const table = document.querySelector('#TableContainer table');
            console.log(newRow);
            table.appendChild(newRow);
        } else {
            console.log('Failed to insert ' + tableName + ' with ' + actualFieldNames[0] + ' - ' + recordDetails[0] + '.');
        }
        bindEditDeleteEvents();
        resetInputfieldNames();
    } catch (error) {
        console.error('Error:', error);
    }
}

async function getNewRecord(pRecordId) {
    const getRecordUrl = 'GetAllRecords.jsp?TableName=' + tableName + '&FieldName=' + actualFieldNames[0] + '&FieldValue=' + pRecordId;
    const response = await fetch(getRecordUrl);
    const data = await response.json();
    const orderedData = reorderJson(data);
    return orderedData;
}

function convertJsonToTable(pJsonData) {
    let table = '<table id="Table">';
    table += '<tr>';
    for (let key in pJsonData[0]) {
        table += '<th>' + key + '</th>';
    }
    table += '</tr>';
    pJsonData.forEach(record => {
        table += '<tr>';
        for (let key in record) {
            table += '<td>' + record[key] + '</td>';
        }
        table += '</tr>';
    });
    table += '</table>';
    return table;
}

async function showAllRecords() {
    try {
        const showAllRecordsUrl = 'GetAllRecords.jsp?TableName=' + tableName;
        const response = await fetch(showAllRecordsUrl);
        const jsonResponse = await response.json();
        const orderedData = reorderJson(jsonResponse);
        const table = convertJsonToTable(orderedData);
        document.getElementById('TableContainer').innerHTML = table;
        bindEditDeleteEvents();
    } catch (error) {
        console.error('Error:', error);
    }
}

function resetInputfieldNames() {
    document.querySelectorAll('input[type="text"]').forEach(input => input.value = '');
    var Operation = 'Save';
    changeButtonText(Operation);
    makeFirstFieldEditable();
}

function makeFirstFieldEditable() {
    const firstInputField = document.getElementById(actualFieldNames[0]);
    firstInputField.readOnly = false;
}

function changeButtonText(pOperation) {
    const saveButton = document.querySelector('button');
    saveButton.textContent = pOperation;
}

function bindEditDeleteEvents() {
    document.querySelectorAll('#EditButton').forEach(button => {
        button.addEventListener('click', populateRecord);
    });
    document.querySelectorAll('#DeleteButton').forEach(button => {
        const RecordId = button.closest('tr').cells[0].innerHTML;
        button.addEventListener('click', () => deleteRecord(RecordId));
    });
}

function populateRecord(event) {
    const row = event.target.closest('tr');
    actualFieldNames.forEach((fieldName, index) => {
        const inputField = document.getElementById(fieldName);
        inputField.value = row.cells[index].innerHTML;
        if (index === 0) {
            inputField.readOnly = true;
        }
    });
    var Operation = 'Update';
    changeButtonText(Operation);
    window.scrollTo(0, 0);
}

async function deleteRecord(pRecordId) {
    if (confirm('Are you sure want to delete?')) {
        try {
            const deleteUrl = 'DeleteRecord.jsp?TableName=' + tableName + '&FieldName=' + actualFieldNames[0] + '&FieldValue=' + pRecordId;
            const response = await fetch(deleteUrl);
            const data = await response.text();
            console.log(data);
            if (data.trim() === "1") {
                console.log(tableName + ' with ' + actualFieldNames[0] + ' - ' + pRecordId + ' deleted successfully.');
                deleteRecordLocally(pRecordId);
            } else {
                console.error('Failed to delete ' + tableName + ' with ' + actualFieldNames[0] + ' - ' + pRecordId + '.');
            }
        } catch (error) {
            console.error('Error:', error);
        }
    } else {
        console.log('Deletion is canceled.');
    }
}

function deleteRecordLocally(pRecordId) {
    const rowToDelete = Array.from(document.querySelectorAll('#TableContainer table tr')).find(row => row.cells[0].innerHTML === pRecordId);
    console.log(rowToDelete);
    rowToDelete.remove();
}