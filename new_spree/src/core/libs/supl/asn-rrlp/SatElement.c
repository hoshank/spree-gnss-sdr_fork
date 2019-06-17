/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "RRLP-Components"
 * 	found in "../rrlp-components.asn"
 */

#include "SatElement.h"

static int
memb_iode_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 239)) {
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
memb_udre_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 3)) {
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
memb_pseudoRangeCor_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -2047 && value <= 2047)) {
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
memb_rangeRateCor_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -127 && value <= 127)) {
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
memb_deltaPseudoRangeCor2_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -127 && value <= 127)) {
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
memb_deltaRangeRateCor2_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -7 && value <= 7)) {
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
memb_deltaPseudoRangeCor3_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -127 && value <= 127)) {
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
memb_deltaRangeRateCor3_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -7 && value <= 7)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t ASN_PER_MEMB_IODE_CONSTR_3 = {
	{ APC_CONSTRAINED,	 8,  8,  0,  239 }	/* (0..239) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t ASN_PER_MEMB_UDRE_CONSTR_4 = {
	{ APC_CONSTRAINED,	 2,  2,  0,  3 }	/* (0..3) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t ASN_PER_MEMB_PSEUDO_RANGE_COR_CONSTR_5 = {
	{ APC_CONSTRAINED,	 12,  12, -2047,  2047 }	/* (-2047..2047) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t ASN_PER_MEMB_RANGE_RATE_COR_CONSTR_6 = {
	{ APC_CONSTRAINED,	 8,  8, -127,  127 }	/* (-127..127) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t ASN_PER_MEMB_DELTA_PSEUDO_RANGE_COR2_CONSTR_7 = {
	{ APC_CONSTRAINED,	 8,  8, -127,  127 }	/* (-127..127) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t ASN_PER_MEMB_DELTA_RANGE_RATE_COR2_CONSTR_8 = {
	{ APC_CONSTRAINED,	 4,  4, -7,  7 }	/* (-7..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t ASN_PER_MEMB_DELTA_PSEUDO_RANGE_COR3_CONSTR_9 = {
	{ APC_CONSTRAINED,	 8,  8, -127,  127 }	/* (-127..127) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t ASN_PER_MEMB_DELTA_RANGE_RATE_COR3_CONSTR_10 = {
	{ APC_CONSTRAINED,	 4,  4, -7,  7 }	/* (-7..7) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_SatElement_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SatElement, satelliteID),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SatelliteID,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"satelliteID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SatElement, iode),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_iode_constraint_1,
		&ASN_PER_MEMB_IODE_CONSTR_3,
		0,
		"iode"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SatElement, udre),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_udre_constraint_1,
		&ASN_PER_MEMB_UDRE_CONSTR_4,
		0,
		"udre"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SatElement, pseudoRangeCor),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_pseudoRangeCor_constraint_1,
		&ASN_PER_MEMB_PSEUDO_RANGE_COR_CONSTR_5,
		0,
		"pseudoRangeCor"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SatElement, rangeRateCor),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_rangeRateCor_constraint_1,
		&ASN_PER_MEMB_RANGE_RATE_COR_CONSTR_6,
		0,
		"rangeRateCor"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SatElement, deltaPseudoRangeCor2),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_deltaPseudoRangeCor2_constraint_1,
		&ASN_PER_MEMB_DELTA_PSEUDO_RANGE_COR2_CONSTR_7,
		0,
		"deltaPseudoRangeCor2"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SatElement, deltaRangeRateCor2),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_deltaRangeRateCor2_constraint_1,
		&ASN_PER_MEMB_DELTA_RANGE_RATE_COR2_CONSTR_8,
		0,
		"deltaRangeRateCor2"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SatElement, deltaPseudoRangeCor3),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_deltaPseudoRangeCor3_constraint_1,
		&ASN_PER_MEMB_DELTA_PSEUDO_RANGE_COR3_CONSTR_9,
		0,
		"deltaPseudoRangeCor3"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SatElement, deltaRangeRateCor3),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_deltaRangeRateCor3_constraint_1,
		&ASN_PER_MEMB_DELTA_RANGE_RATE_COR3_CONSTR_10,
		0,
		"deltaRangeRateCor3"
		},
};
static ber_tlv_tag_t asn_DEF_SatElement_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_SatElement_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* satelliteID at 664 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* iode at 667 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* udre at 669 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* pseudoRangeCor at 673 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* rangeRateCor at 677 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* deltaPseudoRangeCor2 at 680 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* deltaRangeRateCor2 at 683 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* deltaPseudoRangeCor3 at 686 */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 } /* deltaRangeRateCor3 at 689 */
};
static asn_SEQUENCE_specifics_t asn_SPC_SatElement_specs_1 = {
	sizeof(struct SatElement),
	offsetof(struct SatElement, _asn_ctx),
	asn_MAP_SatElement_tag2el_1,
	9,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_SatElement = {
	"SatElement",
	"SatElement",
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
	asn_DEF_SatElement_tags_1,
	sizeof(asn_DEF_SatElement_tags_1)
		/sizeof(asn_DEF_SatElement_tags_1[0]), /* 1 */
	asn_DEF_SatElement_tags_1,	/* Same as above */
	sizeof(asn_DEF_SatElement_tags_1)
		/sizeof(asn_DEF_SatElement_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_SatElement_1,
	9,	/* Elements count */
	&asn_SPC_SatElement_specs_1	/* Additional specs */
};

