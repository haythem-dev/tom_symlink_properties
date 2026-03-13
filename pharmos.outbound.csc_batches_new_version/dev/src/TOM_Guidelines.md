<style type="text/css">
  body
  {
    background-color: #7B7D7D;
  }
</style>

# TOM (ABBA-UW) | how to's and problem solutions


<br>
<br>

## How to's

<br>
<br>

<br>
<br>

## Solution to problems

<br>
<br>

**transfer order could not be processed**
>If the ticket is from PINT: formatting the message into our inhouse format failed.  
>If so, you just need to send a mail with the information, as in the last steps described further down.  
>If the ticket is from APX/csc_batches_edi_input:  
>Error in the transfer order message, which leads to an error in the csc_batches_edi_input application.  

>Here is how to handle such situations:  
>Take a look into the log files of the csc_batches_edi_input application to get the reason why the process stopped.  
>Files that have been processed successfully have a ".OK" at the end of the filename.  
>They are moved automatically in the directory for the current day.  
>Search the file that caused the process to stop and rename it:  
>add ".ERROR" at the end of the file and move it into the directory for the current day.  

>Find out from which manufacturer the transfer order is coming from.  
>The segment with the manfucturer looks like this:  

>Germany: 04MF 21194883 BFA  
>SQL: select * from bliefer where mailnr_1 = 21194883  
>Switzerland: NADMF 7601001010505    F. UHLMANN EYRAUD SA  
>SQL: select * from bliefer where ean1 = 7601001010505  


>Send a mail with the information what caused the error (missing segment, wrong BFA number, no articles etc.)  
>and from which manufacturer the transfer order is to the following:  
>Germany: Christian Renner - C.RENNER@phoenixgroup.eu  
>Switzerland: ksc.uw-om@amedis.ch  

>After the manufacturer has been informed you can close the ticket.

<br>
