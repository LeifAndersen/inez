#include "scip_utils.hpp"

#include "scip/debug.h"

// #define DEBUG

// FIXME: copy-pasted function
inline const char* var_id(SCIP_VAR *v) {
  if (v) return SCIPvarGetName(v);
  return NULL;
}

SCIP_VAR* scip_dvar(SCIP* scip, SCIP_VAR* a, SCIP_VAR* b)
{

  assert(a != b);
  
  static unsigned int n = 0;
  SCIP_CONS* cons;
  SCIP_VAR* rval;
  char cons_id[64], rval_id[64];
  SCIP_VAR* vars[3] = {b, a, NULL};
  SCIP_Real coefs[3] = {1, -1, 1};
  SCIP_Real oo = SCIPinfinity(scip);

  sprintf(rval_id, "dvar%d", n);
  sprintf(cons_id, "deq%d", n++);
  sa(SCIPcreateVarBasic(scip, &rval, rval_id, -oo, oo, 0,
                        SCIP_VARTYPE_INTEGER));
  sa(SCIPaddVar(scip, rval));
  vars[2] = rval;
  sa(SCIPcreateConsLinear 
     (scip, &cons, cons_id, 3, vars, coefs, 0, 0,
      TRUE, TRUE, TRUE, TRUE, TRUE,
      FALSE, FALSE, FALSE, FALSE, FALSE));
  sa(SCIPaddCons(scip, cons));

  return rval;

}

bool scip_fix_variable
(SCIP* scip, SCIP_VAR* v, llint x, bool* infeasible)
{

  SCIP_Bool
    lb_infeasible, lb_tightened,
    ub_infeasible, ub_tightened;

  if (SCIPisEQ(scip, x, SCIPvarGetLbLocal(v)) &&
      SCIPisEQ(scip, x, SCIPvarGetUbLocal(v))) {
    *infeasible = false;
    return false;
  }

  sa(SCIPinferVarUbCons(scip, v, x, NULL, 0, TRUE,
                        &lb_infeasible, &lb_tightened));
  sa(SCIPinferVarLbCons(scip, v, x, NULL, 0, TRUE,
                        &ub_infeasible, &ub_tightened));

  *infeasible = *infeasible || lb_infeasible || ub_infeasible;
  return (lb_tightened || ub_tightened);

}

void scip_branch_around_val(SCIP* scip, SCIP_VAR* v, llint x)
{

#ifdef DEBUG
  cout << "[SU] branching around "
       << x << " for " << var_id(v)
       << endl;
#endif

  fflush(stdout);

  SCIP_NODE* left;
  SCIP_NODE* middle;
  SCIP_NODE* right;
  
  assert(SCIPvarGetLbLocal(v) <= x);
  assert(x <= SCIPvarGetUbLocal(v));
  sa(SCIPbranchVarVal(scip, v, x, &left, &middle, &right));

#ifdef DEBUG
  cout << "created children "
       << left << " < " << middle << " < " << right << endl;
#endif

}

/** includes default SCIP plugins into SCIP */
SCIP_RETCODE SCIPincludeDefaultPlugins_modified(SCIP* scip)
{

   /* nonlinear must be before linear, quadratic, abspower, and and
      due to constraint upgrading */
   sa(SCIPincludeConshdlrNonlinear(scip));
   /* quadratic must be before linear due to constraint upgrading */
   sa(SCIPincludeConshdlrQuadratic(scip));
   /* linear must be before its specializations due to constraint
      upgrading */
   sa(SCIPincludeConshdlrLinear(scip));
   /* absolute power needs to be after quadratic and nonlinear due to
      constraint upgrading */
   sa(SCIPincludeConshdlrAbspower(scip));
   sa(SCIPincludeConshdlrAnd(scip));
   /* bivariate needs to be after quadratic and nonlinear due to
      constraint upgrading */
   sa(SCIPincludeConshdlrBivariate(scip));
   sa(SCIPincludeConshdlrBounddisjunction(scip));
   sa(SCIPincludeConshdlrConjunction(scip));
   sa(SCIPincludeConshdlrCountsols(scip));
   sa(SCIPincludeConshdlrCumulative(scip));
   sa(SCIPincludeConshdlrDisjunction(scip));
   sa(SCIPincludeConshdlrIndicator(scip));
   sa(SCIPincludeConshdlrIntegral(scip));
   sa(SCIPincludeConshdlrKnapsack(scip));
   sa(SCIPincludeConshdlrLinking(scip));
   sa(SCIPincludeConshdlrLogicor(scip));
   sa(SCIPincludeConshdlrOr(scip));
   sa(SCIPincludeConshdlrOrbitope(scip));
   sa(SCIPincludeConshdlrPseudoboolean(scip));
   sa(SCIPincludeConshdlrSetppc(scip));
   /* SOC needs to be after quadratic due to constraint upgrading */
   sa(SCIPincludeConshdlrSOC(scip));
   sa(SCIPincludeConshdlrSOS1(scip));
   sa(SCIPincludeConshdlrSOS2(scip));
   sa(SCIPincludeConshdlrSuperindicator(scip));
   sa(SCIPincludeConshdlrVarbound(scip));
   sa(SCIPincludeConshdlrXor(scip));
   sa(SCIPincludeReaderBnd(scip));
   sa(SCIPincludeReaderCcg(scip));
   sa(SCIPincludeReaderCip(scip));
   sa(SCIPincludeReaderCnf(scip));
   sa(SCIPincludeReaderFix(scip));
   sa(SCIPincludeReaderFzn(scip));
   sa(SCIPincludeReaderGms(scip));
   sa(SCIPincludeReaderLp(scip));
   sa(SCIPincludeReaderMps(scip));
   sa(SCIPincludeReaderOpb(scip));
   sa(SCIPincludeReaderOsil(scip));
   sa(SCIPincludeReaderPip(scip));
   sa(SCIPincludeReaderPpm(scip));
   sa(SCIPincludeReaderPbm(scip));
   sa(SCIPincludeReaderRlp(scip));
   sa(SCIPincludeReaderSol(scip));
   sa(SCIPincludeReaderWbo(scip));
   sa(SCIPincludeReaderZpl(scip));
   sa(SCIPincludePresolBoundshift(scip));
   sa(SCIPincludePresolComponents(scip));
   sa(SCIPincludePresolConvertinttobin(scip));
   sa(SCIPincludePresolDomcol(scip));
   sa(SCIPincludePresolDualinfer(scip));
   sa(SCIPincludePresolGateextraction(scip));
   sa(SCIPincludePresolImplics(scip));
   sa(SCIPincludePresolInttobinary(scip));
   sa(SCIPincludePresolTrivial(scip));
   sa(SCIPincludeNodeselBfs(scip));
   sa(SCIPincludeNodeselBreadthfirst(scip));
   sa(SCIPincludeNodeselDfs(scip));
   sa(SCIPincludeNodeselEstimate(scip));
   sa(SCIPincludeNodeselHybridestim(scip));
   sa(SCIPincludeNodeselRestartdfs(scip));
   sa(SCIPincludeNodeselUct(scip));
   sa(SCIPincludeBranchruleAllfullstrong(scip));
   sa(SCIPincludeBranchruleCloud(scip));
   sa(SCIPincludeBranchruleFullstrong(scip));
   sa(SCIPincludeBranchruleInference(scip));
   sa(SCIPincludeBranchruleLeastinf(scip));
   sa(SCIPincludeBranchruleMostinf(scip));
   sa(SCIPincludeBranchrulePscost(scip));
   sa(SCIPincludeBranchruleRandom(scip));
   sa(SCIPincludeBranchruleRelpscost(scip));
   sa(SCIPincludeHeurActconsdiving(scip));
   sa(SCIPincludeHeurClique(scip));
   sa(SCIPincludeHeurCoefdiving(scip));
   sa(SCIPincludeHeurCrossover(scip));
   sa(SCIPincludeHeurDins(scip));
   sa(SCIPincludeHeurDualval(scip));
   sa(SCIPincludeHeurFeaspump(scip));
   sa(SCIPincludeHeurFixandinfer(scip));
   sa(SCIPincludeHeurFracdiving(scip));
   sa(SCIPincludeHeurGuideddiving(scip));
   sa(SCIPincludeHeurZeroobj(scip));
   sa(SCIPincludeHeurIntdiving(scip));
   sa(SCIPincludeHeurIntshifting(scip));
   sa(SCIPincludeHeurLinesearchdiving(scip));
   sa(SCIPincludeHeurLocalbranching(scip));
   sa(SCIPincludeHeurNlpdiving(scip));
   sa(SCIPincludeHeurMutation(scip));
   sa(SCIPincludeHeurObjpscostdiving(scip));
   sa(SCIPincludeHeurOctane(scip));
   sa(SCIPincludeHeurOneopt(scip));
   sa(SCIPincludeHeurProximity(scip));
   sa(SCIPincludeHeurPscostdiving(scip));
   sa(SCIPincludeHeurRandrounding(scip));
   sa(SCIPincludeHeurRens(scip));
   sa(SCIPincludeHeurRins(scip));
   sa(SCIPincludeHeurRootsoldiving(scip));
   sa(SCIPincludeHeurRounding(scip));
   // sa(SCIPincludeHeurShiftandpropagate(scip));
   sa(SCIPincludeHeurShifting(scip));
   sa(SCIPincludeHeurSimplerounding(scip));
   sa(SCIPincludeHeurSubNlp(scip));
   sa(SCIPincludeHeurTrivial(scip));
   sa(SCIPincludeHeurTrySol(scip));
   sa(SCIPincludeHeurTwoopt(scip));
   // sa(SCIPincludeHeurUndercover(scip));
   sa(SCIPincludeHeurVbounds(scip));
   sa(SCIPincludeHeurVeclendiving(scip));
   sa(SCIPincludeHeurZirounding(scip));
   sa(SCIPincludePropDualfix(scip));
   sa(SCIPincludePropGenvbounds(scip));
   sa(SCIPincludePropObbt(scip));
   sa(SCIPincludePropProbing(scip));
   sa(SCIPincludePropPseudoobj(scip));
   sa(SCIPincludePropRedcost(scip));   
   sa(SCIPincludePropRootredcost(scip));
   sa(SCIPincludePropVbounds(scip));
   sa(SCIPincludeSepaCGMIP(scip));
   sa(SCIPincludeSepaClique(scip));
   sa(SCIPincludeSepaClosecuts(scip));
   sa(SCIPincludeSepaCmir(scip));
   sa(SCIPincludeSepaFlowcover(scip));
   sa(SCIPincludeSepaGomory(scip));
   sa(SCIPincludeSepaImpliedbounds(scip));
   sa(SCIPincludeSepaIntobj(scip));
   sa(SCIPincludeSepaMcf(scip));
   sa(SCIPincludeSepaOddcycle(scip));
   sa(SCIPincludeSepaRapidlearning(scip));
   sa(SCIPincludeSepaStrongcg(scip));
   sa(SCIPincludeSepaZerohalf(scip));
   sa(SCIPincludeDispDefault(scip));

   sa(SCIPdebugIncludeProp(scip));

   return SCIP_OKAY;
  
}
