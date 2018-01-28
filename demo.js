let x = 100;
let xlink = null;

// Display text when document is loaded.
window.onload = function() {
       //when the document is finished loading, replace everything
       //between the <a ...> </a> tags with the value of splitText
//   document.getElementsByClassName('x_val').forEach((ce) =>
//                                                    {
//       ce.innerHTML=x;
//   });
}

document.addEventListener('DOMContentLoaded', (evt) => 
{
    // const checkallbox = document.getElementById('all');
    // const checkalllabel = document.getElementById('all_label');
    
    let x = Math.round(Math.random() * 100);
    xlink = document.getElementsByClassName('x_val');
    // const xDisplay1 = document.getElementById("x_val_display1");

    if (xlink !== null)
    {
        for (i = 0; i < xlink.length; i++)
        {
            xlink.item(i).innerHTML = x;
        }
    }

    /*checkallbox.addEventListener('change', (evt) => 
    {
        testHelp = 20;

        checkalllabel.innerHTML = checkallbox.checked ? "Clear all" : "Check all";

        const month_checks = document.getElementsByName("month");

        month_checks.forEach((cb) =>
        {
            cb.checked = checkallbox.checked;
        });
    });*/
});