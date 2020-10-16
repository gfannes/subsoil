<map version="freeplane 1.8.0">
<!--To view this file, download free mind mapping software Freeplane from http://freeplane.sourceforge.net -->
<node TEXT="auto-encoder&#xa;audio codec" FOLDED="false" ID="ID_494991099" CREATED="1602064931963" MODIFIED="1602065360519" STYLE="oval">
<font SIZE="18"/>
<hook NAME="MapStyle" zoom="1.331">
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
<hook NAME="AutomaticEdgeColor" COUNTER="4" RULE="ON_BRANCH_CREATION"/>
<node TEXT="CREATE DATA" POSITION="right" ID="ID_1579790592" CREATED="1602065245060" MODIFIED="1602065386538">
<edge COLOR="#ff0000"/>
<node TEXT="Input" ID="ID_377858331" CREATED="1602065530470" MODIFIED="1602065531782">
<node TEXT="all.wav from folder" ID="ID_56416887" CREATED="1602065484775" MODIFIED="1602065543441"/>
<node TEXT="blocksize" ID="ID_823296792" CREATED="1602065535626" MODIFIED="1602081283052"/>
</node>
<node TEXT="Transform" ID="ID_1642982574" CREATED="1602065549942" MODIFIED="1602065562181">
<node TEXT="test tf.signal.dst" ID="ID_1432111780" CREATED="1602065563182" MODIFIED="1602065585029"/>
</node>
<node TEXT="Augment" ID="ID_568470845" CREATED="1602066059499" MODIFIED="1602066077106">
<node TEXT="with perceptual model" ID="ID_399040132" CREATED="1602066077872" MODIFIED="1602066084519"/>
</node>
<node TEXT="Save" ID="ID_117699979" CREATED="1602065682185" MODIFIED="1602065703595"/>
</node>
<node TEXT="CREATE CODEC" POSITION="right" ID="ID_806912824" CREATED="1602065458247" MODIFIED="1602065463903">
<edge COLOR="#ff00ff"/>
<node TEXT="Input" ID="ID_1440998058" CREATED="1602065606846" MODIFIED="1602065612182">
<node TEXT="filename" ID="ID_656166031" CREATED="1602065954009" MODIFIED="1602065959345">
<node TEXT="encode|encdec" ID="ID_110380887" CREATED="1602065966247" MODIFIED="1602065978770">
<node TEXT=".wav file" ID="ID_481130623" CREATED="1602065613159" MODIFIED="1602065615993"/>
</node>
<node TEXT="decode" ID="ID_638298707" CREATED="1602065982733" MODIFIED="1602065986629">
<node TEXT=".aea" ID="ID_1630241735" CREATED="1602065987689" MODIFIED="1602065991409"/>
</node>
</node>
<node TEXT="blocksize" ID="ID_700021934" CREATED="1602065674927" MODIFIED="1602065677679"/>
<node TEXT="mode" ID="ID_427443139" CREATED="1602065791178" MODIFIED="1602065792421">
<node TEXT="create" ID="ID_1035052609" CREATED="1602666112466" MODIFIED="1602666114235"/>
<node TEXT="encode" ID="ID_385589689" CREATED="1602065793409" MODIFIED="1602065801258"/>
<node TEXT="decode" ID="ID_211010395" CREATED="1602065802047" MODIFIED="1602065803465"/>
<node TEXT="encdec" ID="ID_1848528343" CREATED="1602065804077" MODIFIED="1602065807183"/>
</node>
</node>
<node TEXT="Codec" ID="ID_733872386" CREATED="1602065623887" MODIFIED="1602065641825">
<node TEXT="encdec" ID="ID_640597145" CREATED="1602065822888" MODIFIED="1602065824673">
<node TEXT="mock" ID="ID_530731688" CREATED="1602065642779" MODIFIED="1602065645944">
<node TEXT="remove high frequencies" ID="ID_923820318" CREATED="1602065646603" MODIFIED="1602065659575"/>
<node TEXT="add noise" ID="ID_1020354073" CREATED="1602065661437" MODIFIED="1602065666396"/>
</node>
<node TEXT="nn" ID="ID_1107317566" CREATED="1602065711723" MODIFIED="1602065712705">
<node TEXT="load encdec model with correct blocksize" ID="ID_1761349510" CREATED="1602065728211" MODIFIED="1602065868384"/>
<node TEXT="load enc and dec models and combine" ID="ID_208037417" CREATED="1602065852520" MODIFIED="1602065861527"/>
</node>
</node>
<node TEXT="encode" ID="ID_1485770825" CREATED="1602066007208" MODIFIED="1602066008450">
<node TEXT="load encoder part" ID="ID_1915337495" CREATED="1602066010277" MODIFIED="1602066019873"/>
<node TEXT="quantize middle layer" ID="ID_1441030817" CREATED="1602066097438" MODIFIED="1602066107413"/>
<node TEXT="write bistream" ID="ID_1943376625" CREATED="1602066405992" MODIFIED="1602066411624"/>
</node>
<node TEXT="decode" ID="ID_1983233726" CREATED="1602066021241" MODIFIED="1602066023497">
<node TEXT="parse bitstream into middle layer" ID="ID_1299286132" CREATED="1602066422573" MODIFIED="1602066431038"/>
<node TEXT="load decoder part of model" ID="ID_1192102785" CREATED="1602066024498" MODIFIED="1602066419684"/>
</node>
</node>
<node TEXT="Output" ID="ID_1915776280" CREATED="1602065667716" MODIFIED="1602065669465">
<node TEXT="encdec|decode" ID="ID_1470024967" CREATED="1602065875817" MODIFIED="1602065904295">
<node TEXT=".wav file" ID="ID_639275386" CREATED="1602065670714" MODIFIED="1602065674056"/>
</node>
<node TEXT="encode" ID="ID_1355169569" CREATED="1602065885871" MODIFIED="1602065887337">
<node TEXT=".aea" ID="ID_428313609" CREATED="1602065906205" MODIFIED="1602065944311"/>
</node>
</node>
</node>
<node TEXT="CREATE MODEL" POSITION="right" ID="ID_1241278081" CREATED="1602065387363" MODIFIED="1602065392746">
<edge COLOR="#0000ff"/>
<node TEXT="Learn from data" ID="ID_290568630" CREATED="1602065715276" MODIFIED="1602065724401"/>
<node TEXT="Add different noise on middle layer" ID="ID_516469171" CREATED="1602066131426" MODIFIED="1602066148399"/>
</node>
</node>
</map>
