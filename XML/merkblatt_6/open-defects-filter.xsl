<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/bug-tracking">
    <xsl:copy>
      <xsl:for-each select="project">
          <xsl:if test="defect[status='open']">
            <xsl:copy>
              <xsl:copy-of select="defect[status='open']"/>
            </xsl:copy>
          </xsl:if>
      </xsl:for-each>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>