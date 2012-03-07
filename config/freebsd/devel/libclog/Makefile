# New ports collection makefile for:    libclog
# Date created:         March 5, 2012
# Whom:                 Dale Rahn <drahn@conformal.com>
#
# $FreeBSD$
#

PORTNAME=       libclog
PORTVERSION=    0.4.0
CATEGORIES=     devel
MASTER_SITES=   https://opensource.conformal.com/snapshots/clog/ \
		LOCAL/bf
DISTNAME=       clog-${PORTVERSION}

MAINTAINER=     drahn@conformal.com
COMMENT=        CLOG is a configurable logging library

BUILD_DEPENDS=  ${LOCALBASE}/lib/libclens.a:${PORTSDIR}/devel/clens

LICENSE=        ISCL
LICENSE_COMB=   single

.include <bsd.port.mk>
