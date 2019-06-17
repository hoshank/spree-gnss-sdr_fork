/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "RRLP-Components"
 * 	found in "../rrlp-components.asn"
 */

#ifndef _GANSSOrbitModel_H_
#define _GANSSOrbitModel_H_


#include <asn_application.h>

/* Including external dependencies */
#include "NavModel-KeplerianSet.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* Dependencies */
    typedef enum GANSSOrbitModel_PR
    {
        GANSSOrbitModel_PR_NOTHING, /* No components present */
        GANSSOrbitModel_PR_keplerianSet,
        /* Extensions may appear below */

    } GANSSOrbitModel_PR;

    /* GANSSOrbitModel */
    typedef struct GANSSOrbitModel
    {
        GANSSOrbitModel_PR present;
        union GANSSOrbitModel_u
        {
            NavModel_KeplerianSet_t keplerianSet;
            /*
		 * This type is extensible,
		 * possible extensions are below.
		 */
        } choice;

        /* Context for parsing across buffer boundaries */
        asn_struct_ctx_t _asn_ctx;
    } GANSSOrbitModel_t;

    /* Implementation */
    extern asn_TYPE_descriptor_t asn_DEF_GANSSOrbitModel;

#ifdef __cplusplus
}
#endif

#endif /* _GANSSOrbitModel_H_ */
#include <asn_internal.h>
