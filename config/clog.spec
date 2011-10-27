%define name		clog
%define version		0.4.0
%define release		1

Name: 		%{name}
Summary: 	Library that provides various logging services
Version: 	%{version}
Release: 	%{release}
License: 	ISC
Group: 		System Environment/Libraries
URL:		http://opensource.conformal.com/wiki/clog
Source: 	%{name}-%{version}.tar.gz
Buildroot:	%{_tmppath}/%{name}-%{version}-buildroot
Prefix: 	/usr

%description
clog (Copious Logger) is a library that provides various logging services.
It can be used to log information to syslog or stderr at various levels of
verboseness. The user can define his/her own debug/verbosity flags to,
at run-time, determine how verbose an application or daemon is.

%prep
%setup -q

%build
make

%install
make install DESTDIR=$RPM_BUILD_ROOT LOCALBASE=/usr

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
/usr/lib/libclog.so.*


%package devel
Summary: Libraries and header files to develop applications using clog
Group: Development/Libraries
Requires: clens-devel >= 0.0.5

%description devel
This package contains the libraries, include files, and documentation to
develop applications with clog.

%files devel
%defattr(-,root,root)
%doc /usr/share/man/man?/*
/usr/include/clog.h
/usr/lib/libclog.so
/usr/lib/libclog.a

%changelog
* Thu Oct 27 2011 - davec 0.4.0-1
- Make clog.h self-contained
- Add addition C<LEVEL> functions to mirror syslog priority levels
- Modify time output for CLOG_F_DATE to include seconds
- Several man page updates and corrections
- Add build versioning
- Other minor cleanup and bug fixes
* Tue Jul 26 2011 - davec 0.3.5-1
- Add logic to install man pages
- Don't link against clens directly from library
* Tue Jun 21 2011 - davec 0.3.4-1
- Create
