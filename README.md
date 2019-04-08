Secure Shell (SSH) Project
--------------------------
This is a project to play around with Secure Shell (SSH). I am trying to build a SSH Tunnel to the SSH server of my college. In future, I will try to upgrade it to work for any SSH servers.

I have used a third-party library for this project, which can be found at: <a href="http://www.libssh.org">libssh.org</a>

To use this library, I used <a href="https://github.com/Microsoft/vcpkg/">Vcpkg</a> for Windows 10. The steps that I followed on my machine are:
	1. Cloned the git repository:<br>
		> git clone https://github.com/Microsoft/vcpkg.git <br>
		> cd vcpkg<br>
	2. Opened PowerShell with "Run as Administrator"<br>
	3. PS> .\bootstrap-vcpkg.bat<br>
	4. .\vcpkg integrate install<br>
	5. .\vcpkg install libssh (it might take about 20 min to install)<br>
	