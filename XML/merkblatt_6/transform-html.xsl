<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html"/>
  <xsl:template match="/">
    <html><head/>
      <body>
        <h1>Projects with open defects</h1>
        <xsl:apply-templates select="bug-tracking/project"/>
      </body>
    </html>
  </xsl:template>
  <xsl:template match="project[defect/status = 'open']">
    Project <xsl:value-of select="@name"/>
  </xsl:template>
  
<!-- zwei der Standardregeln überschreiben
  <xsl:template match="*|/"/>
  <xsl:template match="text()|@*"/>
-->
</xsl:stylesheet>