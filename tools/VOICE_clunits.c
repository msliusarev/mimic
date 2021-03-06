/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                      Copyright (c) 1999-2000                          */
/*                        All Rights Reserved.                           */
/*                                                                       */
/*  Permission is hereby granted, free of charge, to use and distribute  */
/*  this software and its documentation without restriction, including   */
/*  without limitation the rights to use, copy, modify, merge, publish,  */
/*  distribute, sublicense, and/or sell copies of this work, and to      */
/*  permit persons to whom this work is furnished to do so, subject to   */
/*  the following conditions:                                            */
/*   1. The code must retain the above copyright notice, this list of    */
/*      conditions and the following disclaimer.                         */
/*   2. Any modifications must be clearly marked as such.                */
/*   3. Original authors' names are not deleted.                         */
/*   4. The authors' names are not used to endorse or promote products   */
/*      derived from this software without specific prior written        */
/*      permission.                                                      */
/*                                                                       */
/*  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
/*  THIS SOFTWARE.                                                       */
/*                                                                       */
/*************************************************************************/
/*             Author:  Alan W Black (awb@cs.cmu.edu)                    */
/*               Date:  April 2001                                       */
/*************************************************************************/
/*                                                                       */
/*  A simple clunits/ldom voice defintion			         */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include "mimic.h"
#include "cst_clunits.h"
#include "__MIMICLANG__.h"
#include "__MIMICLEX__.h"

static char *__VOICENAME___unit_name(cst_item *s);

extern cst_clunit_db __VOICENAME___db;
cst_voice *__VOICENAME___clunits = NULL;

extern const cst_cart __VOICENAME___dur_cart;
extern const dur_stat * const __VOICENAME___dur_stats[];

cst_voice *register___VOICENAME__(const char *voxdir)
{
    cst_voice *v;
    cst_lexicon *lex;

    if (__VOICENAME___clunits)
        return __VOICENAME___clunits;  /* Already registered */

    v = new_voice();
    v->name = "__NICKNAME__";

    /* Sets up language specific parameters in the __VOICENAME__. */
    __MIMICLANG___init(v);

    /* Things that weren't filled in already. */
    mimic_feat_set_string(v->features,"name","__VOICENAME__");

    /* Duration model */
    mimic_feat_set(v->features,"dur_cart",cart_val(&__VOICENAME___dur_cart));
    mimic_feat_set(v->features,"dur_stats",dur_stats_val((dur_stats *)__VOICENAME___dur_stats));

    /* Lexicon */
    lex = __MIMICLEX___init();
    mimic_feat_set(v->features,"lexicon",lexicon_val(lex));
    mimic_feat_set(v->features,"postlex_func",uttfunc_val(lex->postlex));

    /* Waveform synthesis */
    mimic_feat_set(v->features,"wave_synth_func",uttfunc_val(&clunits_synth));
    mimic_feat_set(v->features,"clunit_db",clunit_db_val(&__VOICENAME___db));
    mimic_feat_set_int(v->features,"sample_rate",__VOICENAME___db.sts->sample_rate);
    mimic_feat_set_string(v->features,"join_type","simple_join");
    mimic_feat_set_string(v->features,"resynth_type","fixed");

    if ((voxdir != NULL) &&
        (__VOICENAME___db.sts->sts == NULL) &&
        (__VOICENAME___db.sts->sts_paged == NULL) &&
        (__VOICENAME___db.sts->frames == NULL))
        mimic_mmap_clunit_voxdata(voxdir,v);

    /* Unit selection */
    __VOICENAME___db.unit_name_func = __VOICENAME___unit_name;

    __VOICENAME___clunits = v;

    return __VOICENAME___clunits;
}

void unregister___VOICENAME__(cst_voice *vox)
{
    if (vox != __VOICENAME___clunits)
	return;
    delete_voice(vox);
    __VOICENAME___clunits = NULL;
}

static const char *__VOICENAME___nextvoicing(cst_item *s)
{
    if (cst_streq("+",mimic_ffeature_string(s,"n.ph_vc")))
        return "V";
    else if (cst_streq("+",mimic_ffeature_string(s,"n.ph_cvox")))
        return "CVox";
    else
        return "UV";
}

static char *__VOICENAME___unit_name(cst_item *s)
{
    const char *name;
    /* This *is* long enough as long as you don't change external things */
    char cname[30];

    name = mimic_ffeature_string(s,"name");
    /* Comment this out if you have more complex unit names */
#if 1
    if (1 == 1)
        return cst_strdup(name);
    else 
#endif
    if (cst_streq("+",mimic_ffeature_string(s,"ph_vc")))
    {
        cst_sprintf(cname,"%s_%s_%s",name,
                    mimic_ffeature_string(s,"R:SylStructure.parent.stress"),
                    __VOICENAME___nextvoicing(s));
    }
    else 
    {
        cst_sprintf(cname,"%s_%s",name,
                    __VOICENAME___nextvoicing(s));
    }

    return cst_strdup(cname);
}
