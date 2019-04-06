Secure Shell (SSH) Project
--------------------------
This is a project to play around with Secure Shell (SSH). I am trying to build a SSH Tunnel to the SSH server of my college. In future, I will try to upgrade it to work for any SSH servers.

I have used a third-party library for this project, which can be found at: <a href="http://www.libssh.org">LIBSSH</a>
To use this library, I needed <a href="https://github.com/Microsoft/vcpkg/">Vcpkg</a> for Windows 10. The instructions to install Vcpkg is available in their git repository.

After installing Vcpkg, I installed libssh library using the following command from the PowerShell (for windows 10):
	.\vcpkg install libssh
	