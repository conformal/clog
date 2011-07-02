# $clog$

%define name		clog
%define version		0.3.4
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
Prefix: 	/usr/local

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
make install DESTDIR=$RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
/usr/local/lib/libclog.so.*


%package devel
Summary: Libraries and header files to develop applications using clog
Group: Development/Libraries

%description devel
This package contains the libraries, include files, and documentation to
develop applications with clog.

%files devel
%defattr(-,root,root)
%doc /usr/local/share/man/man?/*
/usr/local/include/clog.h
/usr/local/lib/libclog.so
/usr/local/lib/libclog.a

%changelog
* Tue Jun 21 2011 - davec 0.3.4-1
- Create
