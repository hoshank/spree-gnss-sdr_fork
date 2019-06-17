/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "RRLP-Components"
 * 	found in "../rrlp-components.asn"
 */

#include "GANSSIonoStormFlags.h"

static int
memb_ionoStormFlag1_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 1)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_ionoStormFlag2_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 1)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_ionoStormFlag3_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 1)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_ionoStormFlag4_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 1)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_ionoStormFlag5_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 1)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t ASN_PER_MEMB_IONO_STORM_FLAG1_CONSTR_2 = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t ASN_PER_MEMB_IONO_STORM_FLAG2_CONSTR_3 = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t ASN_PER_MEMB_IONO_STORM_FLAG3_CONSTR_4 = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t ASN_PER_MEMB_IONO_STORM_FLAG4_CONSTR_5 = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t ASN_PER_MEMB_IONO_STORM_FLAG5_CONSTR_6 = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_GANSSIonoStormFlags_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct GANSSIonoStormFlags, ionoStormFlag1),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_ionoStormFlag1_constraint_1,
		&ASN_PER_MEMB_IONO_STORM_FLAG1_CONSTR_2,
		0,
		"ionoStormFlag1"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GANSSIonoStormFlags, ionoStormFlag2),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_ionoStormFlag2_constraint_1,
		&ASN_PER_MEMB_IONO_STORM_FLAG2_CONSTR_3,
		0,
		"ionoStormFlag2"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GANSSIonoStormFlags, ionoStormFlag3),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_ionoStormFlag3_constraint_1,
		&ASN_PER_MEMB_IONO_STORM_FLAG3_CONSTR_4,
		0,
		"ionoStormFlag3"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GANSSIonoStormFlags, ionoStormFlag4),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_ionoStormFlag4_constraint_1,
		&ASN_PER_MEMB_IONO_STORM_FLAG4_CONSTR_5,
		0,
		"ionoStormFlag4"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GANSSIonoStormFlags, ionoStormFlag5),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_ionoStormFlag5_constraint_1,
		&ASN_PER_MEMB_IONO_STORM_FLAG5_CONSTR_6,
		0,
		"ionoStormFlag5"
		},
};
static ber_tlv_tag_t asn_DEF_GANSSIonoStormFlags_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_GANSSIonoStormFlags_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* ionoStormFlag1 at 1152 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* ionoStormFlag2 at 1153 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* ionoStormFlag3 at 1154 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* ionoStormFlag4 at 1155 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* ionoStormFlag5 at 1156 */
};
static asn_SEQUENCE_specifics_t asn_SPC_GANSSIonoStormFlags_specs_1 = {
	sizeof(struct GANSSIonoStormFlags),
	offsetof(struct GANSSIonoStormFlags, _asn_ctx),
	asn_MAP_GANSSIonoStormFlags_tag2el_1,
	5,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_GANSSIonoStormFlags = {
	"GANSSIonoStormFlags",
	"GANSSIonoStormFlags",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_GANSSIonoStormFlags_tags_1,
	sizeof(asn_DEF_GANSSIonoStormFlags_tags_1)
		/sizeof(asn_DEF_GANSSIonoStormFlags_tags_1[0]), /* 1 */
	asn_DEF_GANSSIonoStormFlags_tags_1,	/* Same as above */
	sizeof(asn_DEF_GANSSIonoStormFlags_tags_1)
		/sizeof(asn_DEF_GANSSIonoStormFlags_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_GANSSIonoStormFlags_1,
	5,	/* Elements count */
	&asn_SPC_GANSSIonoStormFlags_specs_1	/* Additional specs */
};

