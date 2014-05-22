void fuzzy_rules(){
  FuzzyInput* fuz_pitch = new FuzzyInput(1);

  FuzzySet* positiv = new FuzzySet(-5, 40, 90, 90);
  fuz_pitch->addFuzzySet(positiv); 
  FuzzySet* neutral = new FuzzySet(-65, -25, -5, 25);
  fuz_pitch->addFuzzySet(neutral); 
  FuzzySet* negativ = new FuzzySet(-90, -90, -80, -35);
  fuz_pitch->addFuzzySet(negativ);

  fuzzy->addFuzzyInput(fuz_pitch);

  FuzzyOutput* fuz_velocity = new FuzzyOutput(1);

  FuzzySet* foreward = new FuzzySet(90, 180, 180, 180); 
  fuz_velocity->addFuzzySet(foreward);
  FuzzySet* stop = new FuzzySet(65, 80, 100, 115);
  fuz_velocity->addFuzzySet(stop);
  FuzzySet* backward = new FuzzySet(0, 0, 0, 90);
  fuz_velocity->addFuzzySet(backward);

  fuzzy->addFuzzyOutput(fuz_velocity);

  // FuzzyRule 1
  FuzzyRuleAntecedent* ifFuz_pitchPositiv = new FuzzyRuleAntecedent(); 
  ifFuz_pitchPositiv->joinSingle(positiv); 
  FuzzyRuleConsequent* thenFuz_velocityForeward = new FuzzyRuleConsequent();
  thenFuz_velocityForeward->addOutput(foreward);

  FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifFuz_pitchPositiv,  thenFuz_velocityForeward);
  fuzzy->addFuzzyRule(fuzzyRule01);

  // FuzzyRule 2
  FuzzyRuleAntecedent* ifFuz_pitchNeutral = new FuzzyRuleAntecedent(); 
  ifFuz_pitchNeutral->joinSingle(neutral); 
  FuzzyRuleConsequent* thenFuz_velocityStop = new FuzzyRuleConsequent();
  thenFuz_velocityStop->addOutput(stop);

  FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifFuz_pitchNeutral,  thenFuz_velocityStop);
  fuzzy->addFuzzyRule(fuzzyRule02);

  // FuzzyRule 3
  FuzzyRuleAntecedent* ifFuz_pitchNegativ = new FuzzyRuleAntecedent(); 
  ifFuz_pitchNegativ->joinSingle(negativ); 
  FuzzyRuleConsequent* thenFuz_velocityBackward = new FuzzyRuleConsequent();
  thenFuz_velocityBackward->addOutput(backward);

  FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifFuz_pitchNegativ,  thenFuz_velocityBackward);
  fuzzy->addFuzzyRule(fuzzyRule03);
}
