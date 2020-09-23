<map version="freeplane 1.8.0">
<!--To view this file, download free mind mapping software Freeplane from http://freeplane.sourceforge.net -->
<node TEXT="Hands-On ML" FOLDED="false" ID="ID_288381357" CREATED="1600873842103" MODIFIED="1600874110483" STYLE="oval">
<font SIZE="18"/>
<hook NAME="MapStyle" zoom="1.771">
    <properties edgeColorConfiguration="#808080ff,#ff0000ff,#0000ffff,#00ff00ff,#ff00ffff,#00ffffff,#7c0000ff,#00007cff,#007c00ff,#7c007cff,#007c7cff,#7c7c00ff" fit_to_viewport="false"/>

<map_styles>
<stylenode LOCALIZED_TEXT="styles.root_node" STYLE="oval" UNIFORM_SHAPE="true" VGAP_QUANTITY="24.0 pt">
<font SIZE="24"/>
<stylenode LOCALIZED_TEXT="styles.predefined" POSITION="right" STYLE="bubble">
<stylenode LOCALIZED_TEXT="default" ICON_SIZE="12.0 pt" COLOR="#000000" STYLE="fork">
<font NAME="SansSerif" SIZE="10" BOLD="false" ITALIC="false"/>
</stylenode>
<stylenode LOCALIZED_TEXT="defaultstyle.details"/>
<stylenode LOCALIZED_TEXT="defaultstyle.attributes">
<font SIZE="9"/>
</stylenode>
<stylenode LOCALIZED_TEXT="defaultstyle.note" COLOR="#000000" BACKGROUND_COLOR="#ffffff" TEXT_ALIGN="LEFT"/>
<stylenode LOCALIZED_TEXT="defaultstyle.floating">
<edge STYLE="hide_edge"/>
<cloud COLOR="#f0f0f0" SHAPE="ROUND_RECT"/>
</stylenode>
</stylenode>
<stylenode LOCALIZED_TEXT="styles.user-defined" POSITION="right" STYLE="bubble">
<stylenode LOCALIZED_TEXT="styles.topic" COLOR="#18898b" STYLE="fork">
<font NAME="Liberation Sans" SIZE="10" BOLD="true"/>
</stylenode>
<stylenode LOCALIZED_TEXT="styles.subtopic" COLOR="#cc3300" STYLE="fork">
<font NAME="Liberation Sans" SIZE="10" BOLD="true"/>
</stylenode>
<stylenode LOCALIZED_TEXT="styles.subsubtopic" COLOR="#669900">
<font NAME="Liberation Sans" SIZE="10" BOLD="true"/>
</stylenode>
<stylenode LOCALIZED_TEXT="styles.important">
<icon BUILTIN="yes"/>
</stylenode>
</stylenode>
<stylenode LOCALIZED_TEXT="styles.AutomaticLayout" POSITION="right" STYLE="bubble">
<stylenode LOCALIZED_TEXT="AutomaticLayout.level.root" COLOR="#000000" STYLE="oval" SHAPE_HORIZONTAL_MARGIN="10.0 pt" SHAPE_VERTICAL_MARGIN="10.0 pt">
<font SIZE="18"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,1" COLOR="#0033ff">
<font SIZE="16"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,2" COLOR="#00b439">
<font SIZE="14"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,3" COLOR="#990000">
<font SIZE="12"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,4" COLOR="#111111">
<font SIZE="10"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,5"/>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,6"/>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,7"/>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,8"/>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,9"/>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,10"/>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,11"/>
</stylenode>
</stylenode>
</map_styles>
</hook>
<hook NAME="AutomaticEdgeColor" COUNTER="1" RULE="ON_BRANCH_CREATION"/>
<node TEXT="CH3 CLASSIFICATION" POSITION="right" ID="ID_730182775" CREATED="1600873861662" MODIFIED="1600873879826">
<edge COLOR="#ff0000"/>
<node TEXT="PERFORMANCE MEASURES" ID="ID_631043545" CREATED="1600873891720" MODIFIED="1600873936826">
<node TEXT="Accuracy" ID="ID_1844990336" CREATED="1600873898105" MODIFIED="1600873945264">
<node TEXT="not preferred" ID="ID_1937512452" CREATED="1600873945534" MODIFIED="1600873968877"/>
<node TEXT="especially not when data is skewed" ID="ID_906471001" CREATED="1600873969525" MODIFIED="1600873984362"/>
</node>
<node TEXT="Confusion matrix" ID="ID_1534684362" CREATED="1600873986567" MODIFIED="1600873989956">
<node TEXT="sklearn.metrics.confusion_matrix(act, pred)" ID="ID_665436382" CREATED="1600874013744" MODIFIED="1600874590723">
<node TEXT="rows are actuals" ID="ID_931319451" CREATED="1600874203892" MODIFIED="1600874233034"/>
<node TEXT="cols are predictions" ID="ID_1091116085" CREATED="1600874218408" MODIFIED="1600874229221"/>
<node TEXT="[[TN, FP], [FN, TP]]" ID="ID_1564198075" CREATED="1600874320484" MODIFIED="1600874340359"/>
</node>
<node TEXT="precision=TP/(TP+FP)" ID="ID_1413984902" CREATED="1600874131367" MODIFIED="1600874144670">
<node TEXT="accuracy of positive predictions" ID="ID_455267340" CREATED="1600874371345" MODIFIED="1600874380422"/>
<node TEXT="metrics.precision_score(act, pred)" ID="ID_1476963953" CREATED="1600874536257" MODIFIED="1600874556581"/>
</node>
<node TEXT="recall=sensitivity=TP/(TP+FN)" ID="ID_281693240" CREATED="1600874396080" MODIFIED="1600874408073">
<node TEXT="ratio of positives correctly detected" ID="ID_363679132" CREATED="1600874491190" MODIFIED="1600874501896"/>
<node TEXT="metrics.recall_score(act, pred)" ID="ID_457274383" CREATED="1600874558149" MODIFIED="1600874578992"/>
</node>
<node TEXT="f1=harmonic_mean(precision, recall)" ID="ID_592555602" CREATED="1600875298841" MODIFIED="1600875322151">
<node TEXT="metrics.f1_score(act, pred)" ID="ID_1457847969" CREATED="1600875324580" MODIFIED="1600875335192"/>
</node>
</node>
</node>
</node>
</node>
</map>
