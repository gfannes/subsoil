<map version="freeplane 1.8.0">
<!--To view this file, download free mind mapping software Freeplane from http://freeplane.sourceforge.net -->
<node TEXT="PYTHON ML" FOLDED="false" ID="ID_1526694281" CREATED="1600854228270" MODIFIED="1600854246654" STYLE="oval">
<font SIZE="18"/>
<hook NAME="MapStyle" zoom="1.61">
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
<hook NAME="AutomaticEdgeColor" COUNTER="5" RULE="ON_BRANCH_CREATION"/>
<node TEXT="NUMPY" POSITION="right" ID="ID_1354694745" CREATED="1600854582939" MODIFIED="1600865201653" LINK="https://numpy.org/">
<edge COLOR="#0000ff"/>
<node TEXT="Combine" ID="ID_1919340278" CREATED="1601029327467" MODIFIED="1601029340050">
<node TEXT="np.c_[arylike]" ID="ID_416428061" CREATED="1601029340567" MODIFIED="1601029586638">
<node TEXT="similar to np.hstack()" ID="ID_462933055" CREATED="1601029597575" MODIFIED="1601029608152"/>
</node>
<node TEXT="np.concatenate([a,b], axis=0)" ID="ID_280645860" CREATED="1601029359061" MODIFIED="1601029370569"/>
<node TEXT="np.vstack(arylike)" ID="ID_164077892" CREATED="1601029371795" MODIFIED="1601029572984">
<node TEXT="stacks vertical" ID="ID_1628479500" CREATED="1601029378900" MODIFIED="1601029385594"/>
</node>
<node TEXT="np.hstack(arylike)" ID="ID_1831720806" CREATED="1601029387126" MODIFIED="1601029578695"/>
</node>
</node>
<node TEXT="PANDAS" POSITION="right" ID="ID_1220507891" CREATED="1600854586008" MODIFIED="1600854588453">
<edge COLOR="#00ff00"/>
</node>
<node TEXT="SKLEARN" POSITION="right" ID="ID_1256021788" CREATED="1600854589796" MODIFIED="1600854592347">
<edge COLOR="#ff00ff"/>
<node TEXT="links" ID="ID_1197222894" CREATED="1600854250124" MODIFIED="1600854601781">
<node TEXT="Custom transformer for sklearn" ID="ID_1808604401" CREATED="1600854264793" MODIFIED="1600854298462" LINK="https://towardsdatascience.com/pipelines-custom-transformers-in-scikit-learn-the-step-by-step-guide-with-python-code-4a7d9b068156">
<font BOLD="false"/>
</node>
</node>
<node TEXT="Cross validation" ID="ID_844949149" CREATED="1600872656485" MODIFIED="1600872665965">
<node TEXT="train on k-1 folds, test on other; repeat" ID="ID_674388264" CREATED="1600873678174" MODIFIED="1600873733439"/>
<node TEXT="commands" ID="ID_1821256841" CREATED="1600873723548" MODIFIED="1600873727891">
<node TEXT="model_selection.cross_val_score(model, X, y, cv=10, scoring=&apos;accurary&apos;)" ID="ID_457501989" CREATED="1600872678412" MODIFIED="1600872902451">
<node TEXT="Returns fraction correct per fold" ID="ID_622711945" CREATED="1600872745480" MODIFIED="1600872779044"/>
</node>
<node TEXT="model_selection.cross_val_predict(model, X, y, cv=10, method=&apos;predict&apos;|&apos;decision_function&apos;)" ID="ID_1344437417" CREATED="1600872682613" MODIFIED="1600875820674">
<node TEXT="Returns prediction per sample" ID="ID_1280915310" CREATED="1600872802454" MODIFIED="1600872818058"/>
</node>
<node TEXT="metrics.confusion_matrix(act, pred)" ID="ID_1207326335" CREATED="1600872819094" MODIFIED="1600874611481">
<node TEXT="Returns [[TN, FP], [FN, TP]]" ID="ID_300146202" CREATED="1600872843913" MODIFIED="1600872870114"/>
</node>
</node>
</node>
</node>
<node TEXT="MATPLOTLIB" POSITION="right" ID="ID_757671942" CREATED="1600865171253" MODIFIED="1600865192213" LINK="https://matplotlib.org/">
<edge COLOR="#00ffff"/>
</node>
</node>
</map>
