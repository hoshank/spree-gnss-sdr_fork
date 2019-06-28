/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "ULP-Components"
 * 	found in "../supl-common.asn"
 */

#include "SLPAddress.h"

static asn_per_constraints_t ASN_PER_TYPE_SLP_ADDRESS_CONSTR_1 = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  1,  1,  0,  1 }	/* (0..1,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_SLPAddress_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SLPAddress, choice.iPAddress),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_IPAddress,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"iPAddress"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SLPAddress, choice.fQDN),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_FQDN,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"fQDN"
		},
};
static asn_TYPE_tag2member_t asn_MAP_SLPAddress_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* iPAddress at 44 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* fQDN at 45 */
};
static asn_CHOICE_specifics_t asn_SPC_SLPAddress_specs_1 = {
	sizeof(struct SLPAddress),
	offsetof(struct SLPAddress, _asn_ctx),
	offsetof(struct SLPAddress, present),
	sizeof(((struct SLPAddress *)0)->present),
	asn_MAP_SLPAddress_tag2el_1,
	2,	/* Count of tags in the map */
	0,
	2	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_SLPAddress = {
	"SLPAddress",
	"SLPAddress",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	CHOICE_decode_uper,
	CHOICE_encode_uper,
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	&ASN_PER_TYPE_SLP_ADDRESS_CONSTR_1,
	asn_MBR_SLPAddress_1,
	2,	/* Elements count */
	&asn_SPC_SLPAddress_specs_1	/* Additional specs */
};
