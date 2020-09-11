<map version="freeplane 1.8.0">
<!--To view this file, download free mind mapping software Freeplane from http://freeplane.sourceforge.net -->
<node TEXT="ML steps" FOLDED="false" ID="ID_223402968" CREATED="1599823801578" MODIFIED="1599823837208" STYLE="oval">
<font SIZE="18"/>
<hook NAME="MapStyle">
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
<hook NAME="AutomaticEdgeColor" COUNTER="10" RULE="ON_BRANCH_CREATION"/>
<node TEXT="Understand problem" POSITION="right" ID="ID_107097804" CREATED="1599823839921" MODIFIED="1599823852492">
<edge COLOR="#ff0000"/>
<node TEXT="Business case" ID="ID_1982204750" CREATED="1599823853386" MODIFIED="1599823857437"/>
<node TEXT="How solution will be used" ID="ID_1080814101" CREATED="1599823858367" MODIFIED="1599823867659"/>
</node>
<node TEXT="Load data" POSITION="right" ID="ID_51067988" CREATED="1599823870028" MODIFIED="1599823875152">
<edge COLOR="#0000ff"/>
</node>
<node TEXT="Create test set" POSITION="right" ID="ID_1159142081" CREATED="1599823875468" MODIFIED="1599823881243">
<edge COLOR="#00ff00"/>
<node TEXT="Stable wrt reload and future updates" ID="ID_1519783349" CREATED="1599823882137" MODIFIED="1599823892195"/>
<node TEXT="Stratified wrt most correlated var" ID="ID_1983389113" CREATED="1599826889848" MODIFIED="1599826906644">
<node TEXT="sklearn.model_selection.StratifiedShuffleSplit" ID="ID_484933239" CREATED="1599830084934" MODIFIED="1599830109578"/>
<node TEXT="Remove stratifier again" ID="ID_748128031" CREATED="1599830067421" MODIFIED="1599830075720"/>
</node>
</node>
<node TEXT="Inspect data" POSITION="right" ID="ID_1667738407" CREATED="1599823894129" MODIFIED="1599823896990">
<edge COLOR="#ff00ff"/>
<node TEXT="Raw data" ID="ID_57385980" CREATED="1599824425887" MODIFIED="1599824429784">
<node TEXT="Check that data was loaded correctly" ID="ID_949367252" CREATED="1599824465236" MODIFIED="1599824472885"/>
</node>
<node TEXT="Feature types" ID="ID_832138946" CREATED="1599823904184" MODIFIED="1599824480437">
<node TEXT="Check data types and missing values" ID="ID_1364531323" CREATED="1599824481144" MODIFIED="1599824489750"/>
</node>
<node TEXT="Feature ranges" ID="ID_614902516" CREATED="1599824490893" MODIFIED="1599824494717"/>
<node TEXT="Correlation with target" ID="ID_1672620830" CREATED="1599823931909" MODIFIED="1599823936964"/>
<node TEXT="Visualize data" ID="ID_162399852" CREATED="1599823927045" MODIFIED="1599823931072">
<node TEXT="Histograms and scatterplots" ID="ID_1394858974" CREATED="1599823917875" MODIFIED="1599823926470"/>
<node TEXT="Geographical plots" ID="ID_1100664563" CREATED="1599825488253" MODIFIED="1599825495926"/>
</node>
</node>
<node TEXT="Pimp data" POSITION="right" ID="ID_330511483" CREATED="1599823951973" MODIFIED="1599823954678">
<edge COLOR="#7c0000"/>
<node TEXT="Cleaning" ID="ID_1628231092" CREATED="1599823955656" MODIFIED="1599823958430">
<node TEXT="Fill-in missing values" ID="ID_1980574824" CREATED="1599823960424" MODIFIED="1599823964793">
<node TEXT="pd.dropna(), pd.drop() and pd.fillna()" ID="ID_557511866" CREATED="1599830422443" MODIFIED="1599830432466"/>
<node TEXT="sklearn.impute.SimpleImputer" ID="ID_1075608870" CREATED="1599830446861" MODIFIED="1599830456671"/>
</node>
</node>
<node TEXT="Handle categorical data" ID="ID_1854475219" CREATED="1599823967305" MODIFIED="1599823973421">
<node TEXT="Ordinal values" ID="ID_1597866498" CREATED="1599823974126" MODIFIED="1599823981045"/>
<node TEXT="One-hot encoding" ID="ID_1719883558" CREATED="1599823982966" MODIFIED="1599823992958"/>
<node TEXT="Embedding" ID="ID_812874234" CREATED="1599823993955" MODIFIED="1599823995904"/>
</node>
<node TEXT="Create derived columns" ID="ID_17327694" CREATED="1599824019364" MODIFIED="1599824024901"/>
<node TEXT="Normalize data" ID="ID_1634391457" CREATED="1599824025347" MODIFIED="1599824033008"/>
<node TEXT="Create pipeline for all this" ID="ID_451144344" CREATED="1599824043663" MODIFIED="1599824049814"/>
</node>
<node TEXT="Select performance measure" POSITION="right" ID="ID_1676451412" CREATED="1599823943357" MODIFIED="1599823949487">
<edge COLOR="#00ffff"/>
</node>
<node TEXT="Search good model types" POSITION="right" ID="ID_1530026402" CREATED="1599824064422" MODIFIED="1599824071411">
<edge COLOR="#00007c"/>
</node>
<node TEXT="Fine-tune best models" POSITION="right" ID="ID_584517132" CREATED="1599824087588" MODIFIED="1599824095287">
<edge COLOR="#007c00"/>
<node TEXT="Search hyperparameters" ID="ID_1160935781" CREATED="1599824095697" MODIFIED="1599824101589">
<node TEXT="Grid search" ID="ID_898053357" CREATED="1599824104348" MODIFIED="1599824109177"/>
<node TEXT="Randomized search" ID="ID_1925169631" CREATED="1599824109943" MODIFIED="1599824116212"/>
</node>
<node TEXT="Cross validation" ID="ID_328095475" CREATED="1599824120990" MODIFIED="1599824126456"/>
<node TEXT="Ensemble models" ID="ID_1836430079" CREATED="1599824126998" MODIFIED="1599824131356"/>
<node TEXT="Save model" ID="ID_1124277865" CREATED="1599824139779" MODIFIED="1599824141635"/>
</node>
<node TEXT="Test fine-tuned model on test set" POSITION="right" ID="ID_1759146883" CREATED="1599824143392" MODIFIED="1599824152187">
<edge COLOR="#007c7c"/>
</node>
</node>
</map>
