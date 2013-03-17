Write-Host "Clearing test log (old log renamed to 'log.old')."
Copy-Item test/tests.log test/tests.log.old
Clear-Content test/tests.log

$success = 0

Write-Host "Running Unit Tests:"

foreach($i in Get-ChildItem test -filter "*_test.exe") {
        echo "$ENV:DRMEMORY./test/$i"
        $result = & "$ENV:DRMEMORY./test/$i" 2> test/tests.log
        if($result) {
                Write-Host $i PASS
        }
        else {
                Write-Host "ERROR in test $i."
                $success = 1
        }
}

if($success -eq 1) {
        Write-Host "ERRORS FOUND. Here's test/tests.log`:"
        Write-Host "=================="
        Get-Content test/tests.log
        Write-Host "=================="
        exit 1
}

#Write-Host ""
